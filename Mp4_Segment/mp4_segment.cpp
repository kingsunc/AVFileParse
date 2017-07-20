#include "mp4_segment.h"

#define ATOM_PREAMBLE_SIZE		8	// 头部为8个字节;
#define MAX_TRACKS				8	// 轨道最大数;

struct atom_t
{
	unsigned char	type_[4];
	unsigned int	size_;
	unsigned char*	start_;
	unsigned char*	end_;
};

struct stts_t
{
	uint32_t sample_count_;
	uint32_t sample_duration_;
};

struct ctts_t
{
	uint32_t sample_count_;
	uint32_t sample_offset_;
};

struct stsc_t
{
	uint32_t chunk_;
	uint32_t samples_;
	uint32_t id_;
};

struct stbl_t
{
	unsigned char* start_;
	//stsd stsd_;				// sample description
	unsigned char* stts_;		// decoding time-to-sample
	unsigned char* stss_;		// sync sample
	unsigned char* stsc_;		// sample-to-chunk
	unsigned char* stsz_;		// sample size
	unsigned char* stco_;		// chunk offset
	unsigned char* ctts_;		// composition time-to-sample
};

struct minf_t
{
	unsigned char* start_;
	stbl_t stbl_;
};

struct mdia_t
{
	unsigned char* start_;
	unsigned char* mdhd_;
	minf_t minf_;
	//  hdlr hdlr_;
};

struct chunks_t
{
	unsigned int sample_;	// number of the first sample in the chunk
	unsigned int size_;     // number of samples in the chunk
	int id_;                // for multiple codecs mode - not used
	unsigned int pos_;      // start byte position of chunk
};

struct samples_t
{
	unsigned int pts_;      // decoding/presentation time
	unsigned int size_;     // size in bytes
	unsigned int pos_;      // byte offset
	unsigned int cto_;      // composition time offset
};

struct trak_t
{
	unsigned char* start_;
	unsigned char* tkhd_;
	mdia_t mdia_;

	/* temporary indices */
	unsigned int chunks_count_;
	chunks_t* chunks_;

	unsigned int samples_count_;
	samples_t* samples_;
};

struct moov_t
{
	unsigned char* start_;
	unsigned int tracks_;
	unsigned char* mvhd_;
	trak_t traks_[MAX_TRACKS];
};

// buffer[0]->int8
static int ReadInt8(IN unsigned char const* buffer)
{
	return buffer[0];
}

// buffer[0~4]->int32
static int ReadInt32(IN void const* buffer)
{
	unsigned char* p = (unsigned char*)buffer;
	return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];
}

// int32->buffer[0~4]
static void WriteInt32(OUT void* outbuffer, IN uint32_t value)
{
	unsigned char* p = (unsigned char*)outbuffer;
	p[0] = (unsigned char)((value >> 24) & 0xFF);
	p[1] = (unsigned char)((value >> 16) & 0xFF);
	p[2] = (unsigned char)((value >> 8) & 0xFF);
	p[3] = (unsigned char)((value >> 0) & 0xFF);
}

// get header size
static unsigned int GetHeaderSize(IN unsigned char* pBuff)
{
	return ReadInt32(pBuff);
}

// read header info: pBuff->atom
static unsigned char* ReadHeader(IN unsigned char* pBuff, OUT atom_t* atom)
{
	atom->start_ = pBuff;
	memcpy(&atom->type_[0], &pBuff[4], 4);
	atom->size_ = GetHeaderSize(pBuff);
	atom->end_ = atom->start_ + atom->size_;

	return pBuff + ATOM_PREAMBLE_SIZE;
}

// go to the end of atom;
static unsigned char* ToEnd(IN atom_t const* atom)
{
	return atom->end_;
}

// compare the type
static int CompareType(IN atom_t const* atom, IN const char* type)
{
	return (atom->type_[0] == type[0] && atom->type_[1] == type[1] && atom->type_[2] == type[2] && atom->type_[3] == type[3]);
}

// printf the info of atom;
static void PrintInfo(IN atom_t const* atom)
{
	printf("box(%c%c%c%c, %8d : 0x%08X ~ 0x%08X )\n", atom->type_[0], atom->type_[1], atom->type_[2], atom->type_[3], atom->size_, atom->start_, atom->end_);
}

// get the entries of stts
unsigned int GetSttsEntries(IN unsigned char const* stts)
{
	return ReadInt32(stts + 4);
}

// get the sample_count , sample_duration of stts
void GetSttsSampleInfo(IN unsigned char const* stts, IN unsigned int idx,
	OUT unsigned int* sample_count, OUT unsigned int* sample_duration)
{
	unsigned char const* table = stts + 8 + idx * 8;
	*sample_count = ReadInt32(table);
	*sample_duration = ReadInt32(table + 4);
}

// get the entries of ctts
unsigned int GetCttsEntries(IN unsigned char const* ctts)
{
	return ReadInt32(ctts + 4);
}

// get the sample_count, sample_offset of ctts
void GetCttsSampleInfo(IN unsigned char const* ctts, IN unsigned int idx, 
	OUT unsigned int* sample_count, OUT unsigned int* sample_offset)
{
	unsigned char const* table = ctts + 8 + idx * 8;
	*sample_count = ReadInt32(table);
	*sample_offset = ReadInt32(table + 4);
}

// get the samples of ctts
unsigned int GetCttsSamples(OUT unsigned char const* ctts)
{
	long samples = 0;
	long entries = GetCttsEntries(ctts);
	int i;
	for (i = 0; i != entries; ++i)
	{
		unsigned int sample_count;
		unsigned int sample_offset;
		GetCttsSampleInfo(ctts, i, &sample_count, &sample_offset);
		samples += sample_count;
	}

	return samples;
}

// get the entries of stsc
unsigned int GetStscEntries(IN unsigned char const* stsc)
{
	return ReadInt32(stsc + 4);
}

// read stsc_t: stsc->stsc_tbl
void ReadStsc(IN unsigned char const* stsc, IN unsigned int i, OUT stsc_t* stsc_tbl)
{
	stsc_t* table = (stsc_t*)(stsc + 8);
	stsc_tbl->chunk_ = ReadInt32(&table[i].chunk_) - 1;
	stsc_tbl->samples_ = ReadInt32(&table[i].samples_);
	stsc_tbl->id_ = ReadInt32(&table[i].id_);
}

// get the chunk count of stsc
unsigned int GetStscChunkCount(IN unsigned char* stsc, IN unsigned int sample)
{
	unsigned int entries = ReadInt32(stsc + 4);
	stsc_t* table = (stsc_t*)(stsc + 8);

	if (entries == 0)
	{
		return 0;
	}
	/*else if (entries == 1)
	{
		unsigned int table_samples = ReadInt32(&table[0].samples_);
		unsigned int chunk = (sample + 1) / table_samples;
		return chunk - 1;
	}*/
	else
	{
		unsigned int total = 0;
		unsigned int chunk1 = 1;
		unsigned int chunk1samples = 0;
		unsigned int chunk2entry = 0;
		unsigned int chunk, chunk_sample;

		do
		{
			unsigned int range_samples;
			unsigned int chunk2 = ReadInt32(&table[chunk2entry].chunk_);
			chunk = chunk2 - chunk1;
			range_samples = chunk * chunk1samples;

			if (sample < total + range_samples)
			{
				break;
			}

			chunk1samples = ReadInt32(&table[chunk2entry].samples_);
			chunk1 = chunk2;

			if (chunk2entry < entries)
			{
				chunk2entry++;
				total += range_samples;
			}
		} while (chunk2entry < entries);

		if (chunk1samples)
		{
			unsigned int sample_in_chunk = (sample - total) % chunk1samples;
			if (sample_in_chunk != 0)
			{
				//printf("ERROR: sample must be chunk aligned: %d\n", sample_in_chunk);
			}
			chunk = (sample - total) / chunk1samples + chunk1;
		}
		else
		{
			chunk = 1;
		}

		chunk_sample = total + (chunk - chunk1) * chunk1samples;

		return chunk;
	}

	return 0;
}

// get the samples of stsc
unsigned int GetStscSamples(IN unsigned char* stsc)
{
	unsigned int entries = ReadInt32(stsc + 4);
	stsc_t* table = (stsc_t*)(stsc + 8);
	unsigned int samples = 0;
	for (unsigned int i = 0; i != entries; ++i)
	{
		samples += ReadInt32(&table[i].samples_);
	}
	return samples;
}

// get the entries of stco
unsigned int GetStcoEntries(IN unsigned char const* stco)
{
	return ReadInt32(stco + 4);
}

// get the offset of stco
unsigned int GetStcoOffset(IN unsigned char const* stco, int idx)
{
	uint32_t const* table = (uint32_t const*)(stco + 8);
	return ReadInt32(&table[idx]);
}

// get the sample_size of stsz
unsigned int GetStszSampleSize(IN unsigned char const* stsz)
{
	return ReadInt32(stsz + 4);
}

// get the entries of stsz
unsigned int GetStszEntries(IN unsigned char const* stsz)
{
	return ReadInt32(stsz + 8);
}

// get the size of stsz
unsigned int GetStszSize(IN unsigned char const* stsz, IN unsigned int idx)
{
	uint32_t const* table = (uint32_t const*)(stsz + 12);
	return ReadInt32(&table[idx]);
}

// get the duration of stts
unsigned int GetSttsDuration(IN unsigned char const* stts)
{
	long duration = 0;
	long entries = GetSttsEntries(stts);
	for (int i = 0; i < entries; ++i)
	{
		unsigned int sample_count = 0;
		unsigned int sample_duration = 0;
		GetSttsSampleInfo(stts, i, &sample_count, &sample_duration);
		duration += sample_duration * sample_count;
	}
	return duration;
}

// get the sample_count of stts
unsigned int GetSttsSampleCount(IN unsigned char const* stts)
{
	long samples = 0;
	long entries = GetSttsEntries(stts);
	for (int i = 0; i < entries; ++i)
	{
		unsigned int sample_count = 0;
		unsigned int sample_duration = 0;
		GetSttsSampleInfo(stts, i, &sample_count, &sample_duration);
		samples += sample_count;
	}
	return samples;
}

// get the sample of stts
unsigned int GetSttsSample(IN unsigned char const* stts, IN unsigned int time)
{
	unsigned int ret = 0;
	unsigned int time_count = 0;
	unsigned int entries = GetSttsEntries(stts);
	for (unsigned int stts_index = 0; stts_index != entries; ++stts_index)
	{
		unsigned int sample_count = 0;		// 帧数;
		unsigned int sample_delta = 0;		// 每个帧包含多少样本;
		GetSttsSampleInfo(stts, stts_index, &sample_count, &sample_delta);
		if (time_count + sample_delta * sample_count >= time)
		{
			unsigned int stts_count = (time - time_count) / sample_delta;
			time_count += stts_count * sample_delta;
			ret += stts_count;
			break;
		}
		else
		{
			time_count += sample_delta * sample_count;
			ret += sample_count;
			//      stts_index++;
		}
		//    if(stts_index >= table_.size())
		//      break;
	}
	//  *time = time_count;
	return ret;
}

// get the time of stts
unsigned int GetSttsTime(IN unsigned char const* stts, IN unsigned int sample)
{
	unsigned int ret = 0;
	unsigned int stts_index = 0;
	unsigned int sample_count = 0;

	while (1)
	{
		unsigned int table_sample_count = 0;
		unsigned int table_sample_duration = 0;
		GetSttsSampleInfo(stts, stts_index, &table_sample_count, &table_sample_duration);
		if (sample_count + table_sample_count > sample)
		{
			unsigned int stts_count = (sample - sample_count);
			ret += stts_count * table_sample_duration;
			break;
		}
		else
		{
			sample_count += table_sample_count;
			ret += table_sample_count * table_sample_duration;
			stts_index++;
		}
	}
	return ret;
}

// parse the stbl: buffer->stbl
void ParseStbl(OUT stbl_t* stbl, IN unsigned char* buffer, IN unsigned int size)
{
	atom_t leaf_atom;
	unsigned char* buffer_start = buffer;
	stbl->stss_ = 0;
	stbl->ctts_ = 0;
	stbl->start_ = buffer;
	while (buffer < buffer_start + size)
	{
		buffer = ReadHeader(buffer, &leaf_atom);
		PrintInfo(&leaf_atom);

		if (CompareType(&leaf_atom, "stts"))
		{
			stbl->stts_ = buffer;
		}
		else
		if (CompareType(&leaf_atom, "stss"))
		{
			stbl->stss_ = buffer;
		}
		else
		if (CompareType(&leaf_atom, "stsc"))
		{
			stbl->stsc_ = buffer;
		}
		else
		if (CompareType(&leaf_atom, "stsz"))
		{
			stbl->stsz_ = buffer;
		}
		else
		if (CompareType(&leaf_atom, "stco"))
		{
			stbl->stco_ = buffer;
		}
		else
		if (CompareType(&leaf_atom, "co64"))
		{
			perror("TODO: co64");
		}
		else
		if (CompareType(&leaf_atom, "ctts"))
		{
			stbl->ctts_ = buffer;
		}

		buffer = ToEnd(&leaf_atom);
	}
}

// parse the minf: buffer->minf
void ParseMinf(IN minf_t* minf, IN unsigned char* buffer, IN unsigned int size)
{
	atom_t leaf_atom;
	unsigned char* buffer_start = buffer;
	minf->start_ = buffer;
	while (buffer < buffer_start + size)
	{
		buffer = ReadHeader(buffer, &leaf_atom);

		PrintInfo(&leaf_atom);

		if (CompareType(&leaf_atom, "stbl"))
		{
			ParseStbl(&minf->stbl_, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
		}

		buffer = ToEnd(&leaf_atom);
	}
}

// parse the mdia: buffer->mdia
void ParseMdia(OUT mdia_t* mdia, IN unsigned char* buffer, IN unsigned int size)
{
	atom_t leaf_atom;
	unsigned char* buffer_start = buffer;
	mdia->start_ = buffer;
	while (buffer < buffer_start + size)
	{
		buffer = ReadHeader(buffer, &leaf_atom);
		PrintInfo(&leaf_atom);
		if (CompareType(&leaf_atom, "mdhd"))
		{
			mdia->mdhd_ = buffer;
		}
		else
		if (CompareType(&leaf_atom, "minf"))
		{
			ParseMinf(&mdia->minf_, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
		}

		buffer = ToEnd(&leaf_atom);
	}
}

// init trak
void InitTrak(IN trak_t* trak)
{
	memset(trak, 0, sizeof(trak_t));
}

// clear trak
void ClearTrak(IN trak_t* trak)
{
	if (trak->chunks_)
	{
		free(trak->chunks_);
	}

	if (trak->samples_)
	{
		free(trak->samples_);
	}
}

// parse trak
void ParseTrak(OUT trak_t* trak, IN unsigned char* buffer, IN unsigned int size)
{
	atom_t leaf_atom;
	unsigned char* buffer_start = buffer;
	trak->start_ = buffer;
	while (buffer < buffer_start + size)
	{
		buffer = ReadHeader(buffer, &leaf_atom);
		PrintInfo(&leaf_atom);
		if (CompareType(&leaf_atom, "tkhd"))
		{
			trak->tkhd_ = buffer;
		}
		else
		if (CompareType(&leaf_atom, "mdia"))
		{
			ParseMdia(&trak->mdia_, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
		}

		buffer = ToEnd(&leaf_atom);
	}
}

// init moov
void InitMoov(IN moov_t* moov)
{
	memset(moov, 0, sizeof(moov_t));
}

// clear moov
void ClearMoov(IN moov_t* moov)
{
	for (unsigned int i = 0; i != moov->tracks_; ++i)
	{
		ClearTrak(&moov->traks_[i]);
	}
}

// 
void BuildTrakIndex(IN trak_t* trak)
{
	// from stco, get chunk_count , offset
	void const* stco = trak->mdia_.minf_.stbl_.stco_;
	trak->chunks_count_ = GetStcoEntries((const unsigned char *)stco);
	trak->chunks_ = (chunks_t *)malloc(trak->chunks_count_ * sizeof(chunks_t));
	memset(trak->chunks_, 0, trak->chunks_count_ * sizeof(chunks_t));
	for (unsigned int i = 0; i != trak->chunks_count_; ++i)
	{
		trak->chunks_[i].pos_ = GetStcoOffset((const unsigned char *)stco, i);
	}

	// from stsc, we get sample->chunk map
	{
		void const* stsc = trak->mdia_.minf_.stbl_.stsc_;
		unsigned int chunk_size = trak->chunks_count_;
		unsigned int iEnCount = GetStscEntries((const unsigned char *)stsc);
		while (iEnCount > 0)
		{
			stsc_t stsc_table;
			memset(&stsc_table, 0, sizeof(stsc_table));
			--iEnCount;
			ReadStsc((const unsigned char *)stsc, iEnCount, &stsc_table);

			for (unsigned int j = stsc_table.chunk_; j < chunk_size; j++)
			{
				trak->chunks_[j].id_ = stsc_table.id_;
				trak->chunks_[j].size_ = stsc_table.samples_;
			}
			chunk_size = stsc_table.chunk_;
		}
	}

	// from stsz, we get sample_count and the size of every sample.
	{
		void const* stsz = trak->mdia_.minf_.stbl_.stsz_;
		unsigned int sample_size = GetStszSampleSize((const unsigned char *)stsz);
		unsigned int sample_count = 0;
		{
			unsigned int j;
			for (j = 0; j < trak->chunks_count_; j++)
			{
				trak->chunks_[j].sample_ = sample_count;
				sample_count += trak->chunks_[j].size_;
			}
		}

		if (sample_size == 0)
		{
			trak->samples_count_ = GetStszEntries((const unsigned char *)stsz);
		}
		else
		{
			trak->samples_count_ = sample_count;
		}

		trak->samples_ = (samples_t *)malloc(trak->samples_count_ * sizeof(samples_t));
		memset(trak->samples_, 0, trak->samples_count_ * sizeof(samples_t));
		if (sample_size == 0)
		{
			for (unsigned int i = 0; i != trak->samples_count_; ++i)
			{
				trak->samples_[i].size_ = GetStszSize((const unsigned char *)stsz, i);
			}
		}
		else
		{
			for (unsigned int i = 0; i != trak->samples_count_; ++i)
			{
				trak->samples_[i].size_ = sample_size;
			}
		}
	}

	// calc pts of every sample.
	{
		void const* stts = trak->mdia_.minf_.stbl_.stts_;
		unsigned int iIndex = 0;
		unsigned int entries = GetSttsEntries((const unsigned char *)stts);
		for (unsigned int j = 0; j < entries; j++)
		{
			unsigned int pts = 0;
			unsigned int sample_count = 0;
			unsigned int sample_duration = 0;
			GetSttsSampleInfo((const unsigned char *)stts, j, &sample_count, &sample_duration);
			for (unsigned int i = 0; i < sample_count; i++)
			{
				trak->samples_[iIndex++].pts_ = pts;
				pts += sample_duration;
			}
		}
	}

	// calc composition times:
	{
		void const* ctts = trak->mdia_.minf_.stbl_.ctts_;
		if (ctts)
		{
			unsigned int iIndex = 0;
			unsigned int entries = GetCttsEntries((const unsigned char *)ctts);
			for (unsigned int j = 0; j < entries; j++)
			{
				unsigned int sample_count = 0;
				unsigned int sample_offset = 0;
				GetCttsSampleInfo((const unsigned char *)ctts, j, &sample_count, &sample_offset);
				for (unsigned int i = 0; i < sample_count; i++)
				{
					trak->samples_[iIndex++].cto_ = sample_offset;
				}
			}
		}
	}

	// calc the offsets of every sample.
	{
		unsigned int iIndex = 0;
		for (unsigned int j = 0; j < trak->chunks_count_; j++)
		{
			unsigned int pos = trak->chunks_[j].pos_;
			for (unsigned int i = 0; i < trak->chunks_[j].size_; i++)
			{
				trak->samples_[iIndex++].pos_ = pos;
				pos += trak->samples_[iIndex].size_;
			}
		}
	}
}

// write trak index : 修改trak所有对应关系;
void WriteTrakIndex(OUT trak_t* trak, IN unsigned int start, IN unsigned int end)
{
	// in stts, we write samples sample_count and sample_duration
	{
		unsigned char* stts = trak->mdia_.minf_.stbl_.stts_;
		unsigned int entries = 0;
		stts_t* table = ( stts_t*)(stts + 8);
		for (unsigned int i = start; i != end; ++i)
		{
			unsigned int sample_count = 1;
			unsigned int sample_duration = trak->samples_[i + 1].pts_ - trak->samples_[i].pts_;
			while (i != end - 1)
			{
				if ((trak->samples_[i + 1].pts_ - trak->samples_[i].pts_) != sample_duration)
				{
					break;
				}
				++sample_count;
				++i;
			}
			// write entry
			WriteInt32(&table[entries].sample_count_, sample_count);
			WriteInt32(&table[entries].sample_duration_, sample_duration);
			++entries;
		}
		WriteInt32(stts + 4, entries);
		if (GetSttsSampleCount(stts) != end - start)
		{
			printf("ERROR: stts_get_samples=%d, should be %d\n", GetSttsSampleCount(stts), end - start);
		}	
	}

	// ctts = [entries * [sample_count, sample_offset]
	{
		unsigned char* ctts = trak->mdia_.minf_.stbl_.ctts_;
		if (ctts)
		{
			unsigned int entries = 0;
			ctts_t* table = (ctts_t*)(ctts + 8);
			for (unsigned int i = start; i != end; ++i)
			{
				unsigned int sample_count = 1;
				unsigned int sample_offset = trak->samples_[i].cto_;
				while (i != end - 1)
				{
					if (trak->samples_[i + 1].cto_ != sample_offset)
					{
						break;
					}
					++sample_count;
					++i;
				}
				// write entry
				WriteInt32(&table[entries].sample_count_, sample_count);
				WriteInt32(&table[entries].sample_offset_, sample_offset);
				++entries;
			}
			WriteInt32(ctts + 4, entries);
			if (GetCttsSamples(ctts) != end - start)
			{
				printf("ERROR: ctts_get_samples=%d, should be %d\n", GetCttsSamples(ctts), end - start);
			}
		}
	}

	unsigned int chunk_start = 0;
	unsigned int chunk_end = 0;
	// in stsc, we write simple->chunk map.
	{
		unsigned char* stsc = trak->mdia_.minf_.stbl_.stsc_;
		stsc_t* stsc_table = (stsc_t*)(stsc + 8);
		for (unsigned int i = 0; i != trak->chunks_count_; ++i)
		{
			if (trak->chunks_[i].sample_ + trak->chunks_[i].size_ > start)
			{
				chunk_start = i;
				break;
			}
		}

		for (unsigned int j = 0; j != trak->chunks_count_; ++j)
		{
			if (trak->chunks_[j].sample_ + trak->chunks_[j].size_ > end)
			{
				chunk_end = j;
				break;
			}
		}

		{
			unsigned int stsc_entries = 0;
			unsigned int iIndex = chunk_start;
			unsigned int samples = trak->chunks_[iIndex].sample_ + trak->chunks_[iIndex].size_ - start;
			unsigned int id = trak->chunks_[iIndex].id_;

			// write entry [chunk,samples,id]
			WriteInt32(&stsc_table[stsc_entries].chunk_, 1);
			WriteInt32(&stsc_table[stsc_entries].samples_, samples);
			WriteInt32(&stsc_table[stsc_entries].id_, id);
			++stsc_entries;
			if (iIndex != chunk_end)
			{
				for (iIndex += 1; iIndex != chunk_end; ++iIndex)
				{
					if (trak->chunks_[iIndex].size_ != samples)
					{
						samples = trak->chunks_[iIndex].size_;
						id = trak->chunks_[iIndex].id_;
						WriteInt32(&stsc_table[stsc_entries].chunk_, iIndex - chunk_start + 1);
						WriteInt32(&stsc_table[stsc_entries].samples_, samples);
						WriteInt32(&stsc_table[stsc_entries].id_, id);
						++stsc_entries;
					}
				}
			}
			WriteInt32(stsc + 4, stsc_entries);
		}
	}

	// in stco, we write chunk offset
	// here we only move [chunk_start,chunk_end) to [0, chunk_end-chunk_start).
	{
		unsigned char* stco = trak->mdia_.minf_.stbl_.stco_;
		uint32_t* stco_table = (uint32_t*)(stco + 8);
		memmove(stco_table, &stco_table[chunk_start], (chunk_end - chunk_start) * sizeof(uint32_t));
		WriteInt32(stco + 4, chunk_end - chunk_start);

		WriteInt32(stco_table, trak->samples_[start].pos_);
	}

	// in stss, we write keyframe pos;
	if (trak->mdia_.minf_.stbl_.stss_)
	{
		unsigned char* stss = trak->mdia_.minf_.stbl_.stss_;
		unsigned int entries = ReadInt32(stss + 4);
		uint32_t* table = (uint32_t*)(stss + 8);
		unsigned int stss_start = 0;
		unsigned int stss_end = 0;
		for (unsigned int i = 0; i != entries; ++i)
		{
			if ((unsigned int)ReadInt32(&table[i]) >= start + 1)
			{
				stss_start = i;
				break;
			}
		}
		for (unsigned int i = 0; i != entries; ++i)
		{
			if ((unsigned int)ReadInt32(&table[i]) >= end + 1)
			{
				stss_end = i;
				break;
			}
		}

		for (unsigned int j = stss_start; j != stss_end; ++j)
		{
			unsigned int sync_sample = ReadInt32(&table[j]);
			if (sync_sample >= end + 1)
			{
				break;
			}
			WriteInt32(&table[j - stss_start], sync_sample - start);
		}
		WriteInt32(stss + 4, stss_end - stss_start);
	}

	// in stsz, we write sample_count and the size of every of sample.
	// here we only move [chunk_start,chunk_end) to [0, chunk_end-chunk_start).
	{
		unsigned char* stsz = trak->mdia_.minf_.stbl_.stsz_;
		if (GetStszSampleSize(stsz) == 0)
		{
			uint32_t* table = (uint32_t*)(stsz + 12);
			memmove(table, &table[start], (end - start) * sizeof(uint32_t));
			WriteInt32(stsz + 8, end - start);
		}
	}
}

// prase moov: buffer->moov
int ParseMoov(OUT moov_t* moov, IN unsigned char* buffer, IN unsigned int size)
{
	atom_t leaf_atom;
	unsigned char* buffer_start = buffer;
	moov->start_ = buffer;
	while (buffer < buffer_start + size)
	{
		buffer = ReadHeader(buffer, &leaf_atom);
		PrintInfo(&leaf_atom);
		if (CompareType(&leaf_atom, "cmov"))
		{
			return 0;
		}
		else if (CompareType(&leaf_atom, "mvhd"))
		{
			moov->mvhd_ = buffer;
		}
		else if (CompareType(&leaf_atom, "trak"))
		{
			if (moov->tracks_ == MAX_TRACKS)
				return 0;
			else
			{
				trak_t* trak = &moov->traks_[moov->tracks_];
				InitTrak(trak);
				ParseTrak(trak, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
				++moov->tracks_;
			}
		}
		buffer = ToEnd(&leaf_atom);
	}

	// build the indexing tables
	{
		unsigned int i;
		for (i = 0; i != moov->tracks_; ++i)
		{
			BuildTrakIndex(&moov->traks_[i]);
		}
	}

	return 1;
}

// write stco offsets: 就是把所有chunk的偏移量多加offse;
void WriteStcoOffsets(OUT unsigned char* stco, IN int offset)
{
	unsigned int entries = ReadInt32(stco + 4);
	unsigned int* table = (unsigned int*)(stco + 8);
	for (unsigned int i = 0; i != entries; ++i)
	{
		WriteInt32(&table[i], (ReadInt32(&table[i]) + offset));
	}
}

// write trak offsets
void WriteTrakOffsets(OUT trak_t* trak, IN int offset)
{
	unsigned char* stco = trak->mdia_.minf_.stbl_.stco_;
	WriteStcoOffsets(stco, offset);
}

// write moov offsets
void WriteMoovOffsets(OUT moov_t* moov, IN int offset)
{
	for (unsigned int i = 0; i != moov->tracks_; ++i)
	{
		WriteTrakOffsets(&moov->traks_[i], offset);
	}
}

// get the time_scale of mvhd
long GetMvhdTimeScale(IN unsigned char* mvhd)
{
	int version = ReadInt8(mvhd);
	unsigned char* p = mvhd + (version == 0 ? 12 : 20);
	return ReadInt32(p);
}

// write the duration of mvhd
void WriteMvhdDuration(OUT unsigned char* mvhd, IN long duration)
{
	int version = ReadInt8(mvhd);
	if (version == 0)
	{
		WriteInt32(mvhd + 16, duration);
	}
	else
	{
		perror("mvhd_set_duration");
	}
}

// get the time_scale of mdhd
long GetMdhdTimeScale(IN unsigned char* mdhd)
{
	return ReadInt32(mdhd + 12);
}

// write the duration of mdhd
void WriteMdhdDuration(OUT unsigned char* mdhd, IN unsigned int duration)
{
	WriteInt32(mdhd + 16, duration);
}

// write the duration of tkhd
void WriteTkhdDuration(OUT unsigned char* tkhd, IN unsigned int duration)
{
	int version = ReadInt8(tkhd);
	if (version == 0)
	{
		WriteInt32(tkhd + 20, duration);
	}
	else
	{
		perror("tkhd_set_duration");
	}
}

// get the entries of stss
unsigned int GetStssEntries(IN unsigned char const* stss)
{
	return ReadInt32(stss + 4);
}

// get the sample of stss
unsigned int GetStssSample(IN unsigned char const* stss, IN unsigned int idx)
{
	unsigned char const* p = (stss + 8) + (idx * 4);
	return ReadInt32(p);
}

// get the keyframe(nearest) of stss;
unsigned int GetStssNearestKeyFrame(IN unsigned char const* stss, IN unsigned int sample)
{
	// 在样本数量之前扫描同步样本查找关键帧;
	// scan the sync samples to find the key frame that precedes the sample number
	unsigned int i = 0;
	unsigned int entries = GetStssEntries(stss);
	unsigned int table_sample = 0;
	for (i = 0; i != entries; ++i)
	{
		table_sample = GetStssSample(stss, i);
		if (table_sample >= sample)
		{
			break;
		}
	}
	if (table_sample == sample)
	{
		return table_sample;
	}

	return GetStssSample(stss, i - 1);
}

// get the keyframe(nearest) of stbl;
unsigned int GetStblNearestKeyFrame(IN stbl_t const* stbl, IN unsigned int sample)
{
	// If the sync atom is not present, all samples are implicit sync samples.
	if (!stbl->stss_)
	{
		// 对于音频, 没有关键帧则返回当前;
		return sample;
	}

	return GetStssNearestKeyFrame(stbl->stss_, sample);
}

// seek moov
unsigned int SeekMoov(unsigned char* moov_data, unsigned int size,
	float start_time,
	float end_time,
	unsigned char* &mdat_start,
	unsigned int& mdat_size,
	unsigned int offset)
{
	moov_t* moov = (moov_t *)malloc(sizeof(moov_t));
	InitMoov(moov);
	if (!ParseMoov(moov, moov_data, size))
	{
		ClearMoov(moov);
		free(moov);
		return 0;
	}

	{
		long moov_time_scale = GetMvhdTimeScale(moov->mvhd_);
		unsigned int start = (unsigned int)(start_time * moov_time_scale);
		unsigned int end = (unsigned int)(end_time * moov_time_scale);
		unsigned int bytes_to_beginskip = UINT_MAX;
		unsigned int bytes_to_endskip = UINT_MAX;

		// for every trak, convert seconds to sample (time-to-sample).
		// adjust sample to keyframe
		unsigned int trak_sample_start[MAX_TRACKS] = { 0 };	// 点播拖动start_time时, 帧的起始位置;
		unsigned int trak_sample_end[MAX_TRACKS] = { 0 };	// 点播拖动start_time时, 帧的结束位置;
		unsigned int moov_duration = 0;

		// 这里默认前两个轨道为音、视频轨道;
		// 后面的都设置为free过滤，最后只保留两个轨道;
		if (moov->tracks_ > 2)
		{
			for (unsigned int i = 2; i != moov->tracks_; ++i)
			{
				unsigned char* p = moov->traks_[i].start_ - 4;
				p[0] = 'f';
				p[1] = 'r';
				p[2] = 'e';
				p[3] = 'e';
			}
			moov->tracks_ = 2;
		}

		// 获取帧的起始位置;
		for (unsigned int i = 0; i != moov->tracks_; ++i)
		{
			trak_t* trak = &moov->traks_[i];
			stbl_t* stbl = &trak->mdia_.minf_.stbl_;
			long trak_time_scale = GetMdhdTimeScale(trak->mdia_.mdhd_);
			float moov_to_trak_time = (float)trak_time_scale / (float)moov_time_scale;
			float trak_to_moov_time = (float)moov_time_scale / (float)trak_time_scale;

			{
				int iStartPos = GetSttsSample(stbl->stts_, (unsigned int)(start * moov_to_trak_time));
				iStartPos = GetStblNearestKeyFrame(stbl, iStartPos + 1) - 1;
				trak_sample_start[i] = iStartPos;
				printf("track %d : frame_start=%u\n", i, iStartPos);
			}

			{
				int iEndPos = GetSttsSample(stbl->stts_, (unsigned int)(end * moov_to_trak_time));
				trak_sample_end[i] = iEndPos;
				printf("track %d : frame_end  =%u\n", i, iEndPos);
			}
		}

		for (unsigned int i = 0; i != moov->tracks_; ++i)
		{
			trak_t* trak = &moov->traks_[i];
			stbl_t* stbl = &trak->mdia_.minf_.stbl_;
			unsigned int start_sample = trak_sample_start[i];
			unsigned int end_sample = trak_sample_end[i];

			if (start_sample >= end_sample)
			{
				// 若计算出错退出;
				return 0;
			}
			WriteTrakIndex(trak, start_sample, end_sample);

			// 计算真实数据前后跳过的字节数;
			{
				int iFirstIndex = 0;
				int iLastIndex = trak->samples_count_-1;
				unsigned beginskip = trak->samples_[start_sample].pos_ - trak->samples_[iFirstIndex].pos_;
				unsigned endskip = trak->samples_[iLastIndex].pos_ - trak->samples_[end_sample].pos_;
				if (beginskip < bytes_to_beginskip)
				{
					bytes_to_beginskip = beginskip;
				}
				if (endskip < bytes_to_endskip)
				{
					bytes_to_endskip = endskip;
				}
				printf("Trak begin skip %u bytes\n", beginskip);
				printf("Trak end   skip %u bytes\n", endskip);
			}

			// write duration(播放时间);
			{
				// fixup trak (duration);
				unsigned int trak_duration = GetSttsDuration(stbl->stts_);
				long trak_time_scale = GetMdhdTimeScale(trak->mdia_.mdhd_);
				float trak_to_moov_time = (float)moov_time_scale / (float)trak_time_scale;
				unsigned int duration = (long)((float)trak_duration * trak_to_moov_time);
				WriteMdhdDuration(trak->mdia_.mdhd_, trak_duration);
				WriteTkhdDuration(trak->tkhd_, duration);
				printf("trak: new_duration=%d\n", duration);
				if (duration > moov_duration)
				{
					moov_duration = duration;
				}
			}

			printf("stco.size=%d, ", ReadInt32(stbl->stco_ + 4));
			printf("stts.size=%d samples=%d\n", ReadInt32(stbl->stts_ + 4), GetSttsSampleCount(stbl->stts_));
			printf("stsz.size=%d\n", ReadInt32(stbl->stsz_ + 8));
			printf("stsc.samples=%d\n", GetStscSamples(stbl->stsc_));
		}
		WriteMvhdDuration(moov->mvhd_, moov_duration);

		offset -= bytes_to_beginskip;

		printf("shifting offsets by %d\n", offset);
		WriteMoovOffsets(moov, offset);

		mdat_start += bytes_to_beginskip;
		mdat_size -= (bytes_to_beginskip + bytes_to_endskip);
	}

	ClearMoov(moov);
	free(moov);

	return 1;
}

bool mp4_segment(OUT mp4Buffer& outMp4, mp4Buffer& inMp4, float iStartTime, float iEndTime)
{
	if ((!inMp4.m_pBuff) || (inMp4.m_iSize <= 0) || (iStartTime < 0) || (iEndTime < 0) || (iStartTime > iEndTime))
	{
		return false;
	}

	unsigned char* moov_data = NULL;
	unsigned int size = 10;
	float start_time = iStartTime;
	float end_time = iEndTime;
	unsigned char* mdat_start = NULL;
	unsigned int mdat_size = 0;
	unsigned int offset = 0;

	std::string strOutBuff;
	unsigned int iOutSize = 0;

	unsigned char* pBegin = inMp4.m_pBuff;
	unsigned char* pData = pBegin;
	unsigned char* pEnd = pBegin + inMp4.m_iSize;
	while (pData < pEnd)
	{
		atom_t tempAtom;
		pData = ReadHeader(pData, &tempAtom);
		if (CompareType(&tempAtom, "moov"))
		{
			moov_data = pData;
			size = tempAtom.size_ - ATOM_PREAMBLE_SIZE;
		}
		else if (CompareType(&tempAtom, "mdat"))
		{
			mdat_start = (unsigned char*)pData;
			mdat_size = tempAtom.size_;
		}
		pData += tempAtom.size_ - ATOM_PREAMBLE_SIZE;
	}

	SeekMoov(moov_data, size, start_time, end_time, mdat_start, mdat_size, offset);

	{
		pData = pBegin;
		while (pData < pEnd)
		{
			atom_t tempAtom;
			pData = ReadHeader(pData, &tempAtom);
			if (CompareType(&tempAtom, "moov"))
			{
				// moov;
				strOutBuff.append(std::string((char*)(moov_data - ATOM_PREAMBLE_SIZE), size + ATOM_PREAMBLE_SIZE));
				iOutSize += size + ATOM_PREAMBLE_SIZE;
			}
			else if (CompareType(&tempAtom, "mdat"))
			{
				// mdat;
				unsigned char mdatHeader[ATOM_PREAMBLE_SIZE] = { 0 };
				WriteInt32(&mdatHeader[0], mdat_size + sizeof(mdatHeader));
				memcpy(&mdatHeader[4], "mdat", 4);
				strOutBuff.append(std::string((char*)(mdatHeader), sizeof(mdatHeader)));
				iOutSize += sizeof(mdatHeader);

				strOutBuff.append(std::string((char*)(mdat_start), mdat_size));
				iOutSize += mdat_size;
			}
			else
			{
				strOutBuff.append(std::string((char*)(pData - ATOM_PREAMBLE_SIZE), tempAtom.size_));
				iOutSize += tempAtom.size_;
			}
			pData += tempAtom.size_ - ATOM_PREAMBLE_SIZE;
		}
	}

	outMp4.m_pBuff = new unsigned char[iOutSize];
	outMp4.m_iSize = iOutSize;
	memcpy(outMp4.m_pBuff, strOutBuff.c_str(), iOutSize);

	return true;
}
