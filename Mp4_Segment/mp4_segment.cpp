#include "mp4_segment.h"

#define ATOM_PREAMBLE_SIZE		8	// 头部为8个字节;
#define MAX_TRACKS				8	// 轨道最大数;

static int read_char(unsigned char const* buffer)
{
	return buffer[0];
}

static int read_int32(void const* buffer)
{
	unsigned char* p = (unsigned char*)buffer;
	return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];
}

static void write_int32(void* outbuffer, uint32_t value)
{
	unsigned char* p = (unsigned char*)outbuffer;
	p[0] = (unsigned char)((value >> 24) & 0xff);
	p[1] = (unsigned char)((value >> 16) & 0xff);
	p[2] = (unsigned char)((value >> 8) & 0xff);
	p[3] = (unsigned char)((value >> 0) & 0xff);
}

struct atom_t
{
	unsigned char type_[4];
	unsigned int size_;
	unsigned char* start_;
	unsigned char* end_;
};

static unsigned int atom_header_size(unsigned char* atom_bytes)
{
	return read_int32(atom_bytes);
}

static unsigned char* atom_read_header(unsigned char* buffer, struct atom_t* atom)
{
	atom->start_ = buffer;
	memcpy(&atom->type_[0], &buffer[4], 4);
	atom->size_ = atom_header_size(buffer);
	atom->end_ = atom->start_ + atom->size_;

	return buffer + ATOM_PREAMBLE_SIZE;
}

static unsigned char* atom_skip(struct atom_t const* atom)
{
	return atom->end_;
}

static int atom_is(struct atom_t const* atom, const char* type)
{
	return (atom->type_[0] == type[0] && atom->type_[1] == type[1] && atom->type_[2] == type[2] && atom->type_[3] == type[3]);
}

static void atom_print(struct atom_t const* atom)
{
	printf("Atom(%c%c%c%c,%d)\n", atom->type_[0], atom->type_[1], atom->type_[2], atom->type_[3], atom->size_);
}

unsigned int stts_get_entries(unsigned char const* stts)
{
	return read_int32(stts + 4);
}

void stts_get_sample_count_and_duration(unsigned char const* stts,
	unsigned int idx, unsigned int* sample_count, unsigned int* sample_duration)
{
	unsigned char const* table = stts + 8 + idx * 8;
	*sample_count = read_int32(table);
	*sample_duration = read_int32(table + 4);
}

struct stts_table_t
{
	uint32_t sample_count_;
	uint32_t sample_duration_;
};

unsigned int ctts_get_entries(unsigned char const* ctts)
{
	return read_int32(ctts + 4);
}

void ctts_get_sample_count_and_offset(unsigned char const* ctts,
	unsigned int idx, unsigned int* sample_count, unsigned int* sample_offset)
{
	unsigned char const* table = ctts + 8 + idx * 8;
	*sample_count = read_int32(table);
	*sample_offset = read_int32(table + 4);
}

unsigned int ctts_get_samples(unsigned char const* ctts)
{
	long samples = 0;
	long entries = ctts_get_entries(ctts);
	int i;
	for (i = 0; i != entries; ++i)
	{
		unsigned int sample_count;
		unsigned int sample_offset;
		ctts_get_sample_count_and_offset(ctts, i, &sample_count, &sample_offset);
		samples += sample_count;
	}

	return samples;
}

struct ctts_table_t
{
	uint32_t sample_count_;
	uint32_t sample_offset_;
};

struct stsc_table_t
{
	uint32_t chunk_;
	uint32_t samples_;
	uint32_t id_;
};

unsigned int stsc_get_entries(unsigned char const* stsc)
{
	return read_int32(stsc + 4);
}

void stsc_get_table(unsigned char const* stsc, unsigned int i, struct stsc_table_t *stsc_table)
{
	struct stsc_table_t* table = (struct stsc_table_t*)(stsc + 8);
	stsc_table->chunk_ = read_int32(&table[i].chunk_) - 1;
	stsc_table->samples_ = read_int32(&table[i].samples_);
	stsc_table->id_ = read_int32(&table[i].id_);
}

unsigned int stsc_get_chunk(unsigned char* stsc, unsigned int sample)
{
	unsigned int entries = read_int32(stsc + 4);
	struct stsc_table_t* table = (struct stsc_table_t*)(stsc + 8);

	if (entries == 0)
	{
		return 0;
	}
	else
		//  if(entries == 1)
		//  {
		//    unsigned int table_samples = read_int32(&table[0].samples_);
		//    unsigned int chunk = (sample + 1) / table_samples;
		//    return chunk - 1;
		//  }
		//  else
	{
		unsigned int total = 0;
		unsigned int chunk1 = 1;
		unsigned int chunk1samples = 0;
		unsigned int chunk2entry = 0;
		unsigned int chunk, chunk_sample;

		do
		{
			unsigned int range_samples;
			unsigned int chunk2 = read_int32(&table[chunk2entry].chunk_);
			chunk = chunk2 - chunk1;
			range_samples = chunk * chunk1samples;

			if (sample < total + range_samples)
			{
				break;
			}

			chunk1samples = read_int32(&table[chunk2entry].samples_);
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
}

unsigned int stsc_get_samples(unsigned char* stsc)
{
	unsigned int entries = read_int32(stsc + 4);
	struct stsc_table_t* table = (struct stsc_table_t*)(stsc + 8);
	unsigned int samples = 0;
	unsigned int i;
	for (i = 0; i != entries; ++i)
	{
		samples += read_int32(&table[i].samples_);
	}
	return samples;
}

unsigned int stco_get_entries(unsigned char const* stco)
{
	return read_int32(stco + 4);
}

unsigned int stco_get_offset(unsigned char const* stco, int idx)
{
	uint32_t const* table = (uint32_t const*)(stco + 8);
	return read_int32(&table[idx]);
}

unsigned int stsz_get_sample_size(unsigned char const* stsz)
{
	return read_int32(stsz + 4);
}

unsigned int stsz_get_entries(unsigned char const* stsz)
{
	return read_int32(stsz + 8);
}

unsigned int stsz_get_size(unsigned char const* stsz, unsigned int idx)
{
	uint32_t const* table = (uint32_t const*)(stsz + 12);
	return read_int32(&table[idx]);
}

unsigned int stts_get_duration(unsigned char const* stts)
{
	long duration = 0;
	long entries = stts_get_entries(stts);
	int i;
	for (i = 0; i != entries; ++i)
	{
		unsigned int sample_count;
		unsigned int sample_duration;
		stts_get_sample_count_and_duration(stts, i,
			&sample_count, &sample_duration);
		duration += sample_duration * sample_count;
	}

	return duration;
}

unsigned int stts_get_samples(unsigned char const* stts)
{
	long samples = 0;
	long entries = stts_get_entries(stts);
	int i;
	for (i = 0; i != entries; ++i)
	{
		unsigned int sample_count;
		unsigned int sample_duration;
		stts_get_sample_count_and_duration(stts, i,
			&sample_count, &sample_duration);
		samples += sample_count;
	}

	return samples;
}

unsigned int stts_get_sample(unsigned char const* stts, unsigned int time)
{
	unsigned int ret = 0;
	unsigned int time_count = 0;
	unsigned int entries = stts_get_entries(stts);

	for (unsigned int stts_index = 0; stts_index != entries; ++stts_index)
	{
		unsigned int sample_count = 0;		// 帧数;
		unsigned int sample_delta = 0;		// 每个帧包含多少样本;
		stts_get_sample_count_and_duration(stts, stts_index, &sample_count, &sample_delta);
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

unsigned int stts_get_time(unsigned char const* stts, unsigned int sample)
{
	unsigned int ret = 0;
	unsigned int stts_index = 0;
	unsigned int sample_count = 0;

	for (;;)
	{
		unsigned int table_sample_count;
		unsigned int table_sample_duration;
		stts_get_sample_count_and_duration(stts, stts_index,
			&table_sample_count, &table_sample_duration);

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


struct stbl_t
{
	unsigned char* start_;
	//stsd stsd_;               // sample description
	unsigned char* stts_;     // decoding time-to-sample
	unsigned char* stss_;     // sync sample
	unsigned char* stsc_;     // sample-to-chunk
	unsigned char* stsz_;     // sample size
	unsigned char* stco_;     // chunk offset
	unsigned char* ctts_;     // composition time-to-sample
};

void stbl_parse(struct stbl_t* stbl, unsigned char* buffer, unsigned int size)
{
	struct atom_t leaf_atom;
	unsigned char* buffer_start = buffer;
	stbl->stss_ = 0;
	stbl->ctts_ = 0;

	stbl->start_ = buffer;

	while (buffer < buffer_start + size)
	{
		buffer = atom_read_header(buffer, &leaf_atom);

		atom_print(&leaf_atom);

		if (atom_is(&leaf_atom, "stts"))
		{
			stbl->stts_ = buffer;
		}
		else
		if (atom_is(&leaf_atom, "stss"))
		{
			stbl->stss_ = buffer;
		}
		else
		if (atom_is(&leaf_atom, "stsc"))
		{
			stbl->stsc_ = buffer;
		}
		else
		if (atom_is(&leaf_atom, "stsz"))
		{
			stbl->stsz_ = buffer;
		}
		else
		if (atom_is(&leaf_atom, "stco"))
		{
			stbl->stco_ = buffer;
		}
		else
		if (atom_is(&leaf_atom, "co64"))
		{
			perror("TODO: co64");
		}
		else
		if (atom_is(&leaf_atom, "ctts"))
		{
			stbl->ctts_ = buffer;
		}

		buffer = atom_skip(&leaf_atom);
	}
}

struct minf_t
{
	unsigned char* start_;
	struct stbl_t stbl_;
};

void minf_parse(struct minf_t* minf, unsigned char* buffer, unsigned int size)
{
	struct atom_t leaf_atom;
	unsigned char* buffer_start = buffer;

	minf->start_ = buffer;

	while (buffer < buffer_start + size)
	{
		buffer = atom_read_header(buffer, &leaf_atom);

		atom_print(&leaf_atom);

		if (atom_is(&leaf_atom, "stbl"))
		{
			stbl_parse(&minf->stbl_, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
		}

		buffer = atom_skip(&leaf_atom);
	}
}

struct mdia_t
{
	unsigned char* start_;
	unsigned char* mdhd_;
	struct minf_t minf_;
	//  hdlr hdlr_;
};

void mdia_parse(struct mdia_t* mdia, unsigned char* buffer, unsigned int size)
{
	struct atom_t leaf_atom;
	unsigned char* buffer_start = buffer;

	mdia->start_ = buffer;

	while (buffer < buffer_start + size)
	{
		buffer = atom_read_header(buffer, &leaf_atom);

		atom_print(&leaf_atom);

		if (atom_is(&leaf_atom, "mdhd"))
		{
			mdia->mdhd_ = buffer;
		}
		else
		if (atom_is(&leaf_atom, "minf"))
		{
			minf_parse(&mdia->minf_, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
		}

		buffer = atom_skip(&leaf_atom);
	}
}

struct chunks_t
{
	unsigned int sample_;   // number of the first sample in the chunk
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
	struct mdia_t mdia_;

	/* temporary indices */
	unsigned int chunks_size_;
	chunks_t* chunks_;

	unsigned int samples_size_;
	samples_t* samples_;
};

void trak_init(struct trak_t* trak)
{
	memset(trak, 0, sizeof(trak_t));
}

void trak_exit(struct trak_t* trak)
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

void trak_parse(struct trak_t* trak, unsigned char* buffer, unsigned int size)
{
	struct atom_t leaf_atom;
	unsigned char* buffer_start = buffer;

	trak->start_ = buffer;

	while (buffer < buffer_start + size)
	{
		buffer = atom_read_header(buffer, &leaf_atom);

		atom_print(&leaf_atom);

		if (atom_is(&leaf_atom, "tkhd"))
		{
			trak->tkhd_ = buffer;
		}
		else
		if (atom_is(&leaf_atom, "mdia"))
		{
			mdia_parse(&trak->mdia_, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
		}

		buffer = atom_skip(&leaf_atom);
	}
}

struct moov_t
{
	unsigned char* start_;
	unsigned int tracks_;
	unsigned char* mvhd_;
	struct trak_t traks_[MAX_TRACKS];
};

void moov_init(struct moov_t* moov)
{
	memset(moov, 0, sizeof(moov_t));
}

void moov_exit(struct moov_t* moov)
{
	unsigned int i;
	for (i = 0; i != moov->tracks_; ++i)
	{
		trak_exit(&moov->traks_[i]);
	}
}

void trak_build_index(struct trak_t* trak)
{
	void const* stco = trak->mdia_.minf_.stbl_.stco_;

	trak->chunks_size_ = stco_get_entries((const unsigned char *)stco);
	trak->chunks_ = (chunks_t *)malloc(trak->chunks_size_ * sizeof(struct chunks_t));

	{
		unsigned int i;
		for (i = 0; i != trak->chunks_size_; ++i)
		{
			trak->chunks_[i].pos_ = stco_get_offset((const unsigned char *)stco, i);
		}
	}

	// process chunkmap:
	{
		void const* stsc = trak->mdia_.minf_.stbl_.stsc_;
		unsigned int last = trak->chunks_size_;
		unsigned int i = stsc_get_entries((const unsigned char *)stsc);
		while (i > 0)
		{
			struct stsc_table_t stsc_table;
			unsigned int j;

			--i;

			stsc_get_table((const unsigned char *)stsc, i, &stsc_table);
			for (j = stsc_table.chunk_; j < last; j++)
			{
				trak->chunks_[j].id_ = stsc_table.id_;
				trak->chunks_[j].size_ = stsc_table.samples_;
			}
			last = stsc_table.chunk_;
		}
	}

	// calc pts of chunks:
	{
		void const* stsz = trak->mdia_.minf_.stbl_.stsz_;
		unsigned int sample_size = stsz_get_sample_size((const unsigned char *)stsz);
		unsigned int s = 0;
		{
			unsigned int j;
			for (j = 0; j < trak->chunks_size_; j++)
			{
				trak->chunks_[j].sample_ = s;
				s += trak->chunks_[j].size_;
			}
		}

		if (sample_size == 0)
		{
			trak->samples_size_ = stsz_get_entries((const unsigned char *)stsz);
		}
		else
		{
			trak->samples_size_ = s;
		}

		trak->samples_ = (samples_t *)malloc(trak->samples_size_ * sizeof(struct samples_t));

		if (sample_size == 0)
		{
			unsigned int i;
			for (i = 0; i != trak->samples_size_; ++i)
				trak->samples_[i].size_ = stsz_get_size((const unsigned char *)stsz, i);
		}
		else
		{
			unsigned int i;
			for (i = 0; i != trak->samples_size_; ++i)
				trak->samples_[i].size_ = sample_size;
		}
	}

	//  i = 0;
	//  for (j = 0; j < trak->durmap_size; j++)
	//    i += trak->durmap[j].num;
	//  if (i != s) {
	//    mp_msg(MSGT_DEMUX, MSGL_WARN,
	//           "MOV: durmap and chunkmap sample count differ (%i vs %i)\n", i, s);
	//    if (i > s) s = i;
	//  }

	// calc pts:
	{
		void const* stts = trak->mdia_.minf_.stbl_.stts_;
		unsigned int s = 0;
		unsigned int entries = stts_get_entries((const unsigned char *)stts);
		unsigned int j;
		for (j = 0; j < entries; j++)
		{
			unsigned int i;
			unsigned int pts = 0;
			unsigned int sample_count;
			unsigned int sample_duration;
			stts_get_sample_count_and_duration((const unsigned char *)stts, j,
				&sample_count, &sample_duration);
			for (i = 0; i < sample_count; i++)
			{
				trak->samples_[s].pts_ = pts;
				++s;
				pts += sample_duration;
			}
		}
	}

	// calc composition times:
	{
		void const* ctts = trak->mdia_.minf_.stbl_.ctts_;
		if (ctts)
		{
			unsigned int s = 0;
			unsigned int entries = ctts_get_entries((const unsigned char *)ctts);
			unsigned int j;
			for (j = 0; j < entries; j++)
			{
				unsigned int i;
				unsigned int sample_count;
				unsigned int sample_offset;
				ctts_get_sample_count_and_offset((const unsigned char *)ctts, j, &sample_count, &sample_offset);
				for (i = 0; i < sample_count; i++)
				{
					trak->samples_[s].cto_ = sample_offset;
					++s;
				}
			}
		}
	}

	// calc sample offsets
	{
		unsigned int s = 0;
		unsigned int j;
		for (j = 0; j < trak->chunks_size_; j++)
		{
			unsigned int pos = trak->chunks_[j].pos_;
			unsigned int i;
			for (i = 0; i < trak->chunks_[j].size_; i++)
			{
				trak->samples_[s].pos_ = pos;
				pos += trak->samples_[s].size_;
				++s;
			}
		}
	}
}

void trak_write_index(struct trak_t* trak, unsigned int start, unsigned int end)
{
	// write samples [start,end>

	// stts = [entries * [sample_count, sample_duration]
	{
		unsigned char* stts = trak->mdia_.minf_.stbl_.stts_;
		unsigned int entries = 0;
		struct stts_table_t* table = (struct stts_table_t*)(stts + 8);
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
			write_int32(&table[entries].sample_count_, sample_count);
			write_int32(&table[entries].sample_duration_, sample_duration);
			++entries;
		}
		write_int32(stts + 4, entries);
		if (stts_get_samples(stts) != end - start)
		{
			printf("ERROR: stts_get_samples=%d, should be %d\n", stts_get_samples(stts), end - start);
		}
	}

	// ctts = [entries * [sample_count, sample_offset]
	{
	unsigned char* ctts = trak->mdia_.minf_.stbl_.ctts_;
	if (ctts)
	{
		unsigned int entries = 0;
		struct ctts_table_t* table = (struct ctts_table_t*)(ctts + 8);
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
			write_int32(&table[entries].sample_count_, sample_count);
			write_int32(&table[entries].sample_offset_, sample_offset);
			++entries;
		}
		write_int32(ctts + 4, entries);
		if (ctts_get_samples(ctts) != end - start)
		{
			printf("ERROR: ctts_get_samples=%d, should be %d\n", ctts_get_samples(ctts), end - start);
		}
	}
}

	// process chunkmap:
	{
		unsigned char* stsc = trak->mdia_.minf_.stbl_.stsc_;
		struct stsc_table_t* stsc_table = (struct stsc_table_t*)(stsc + 8);
		unsigned int i = 0;
		unsigned int j = 0;
		for (i = 0; i != trak->chunks_size_; ++i)
		{
			if (trak->chunks_[i].sample_ + trak->chunks_[i].size_ > start)
			{
				break;
			}
		}
		for (j = trak->chunks_size_-1; j >= 0; j--)
		{
			if (trak->chunks_[j].sample_ + trak->chunks_[i].size_ < end)
			{
				break;
			}
		}

		{
			unsigned int stsc_entries = 0;
			unsigned int chunk_start = i;
			unsigned int chunk_end = j;
			unsigned int samples = trak->chunks_[i].sample_ + trak->chunks_[i].size_ - start;
			unsigned int id = trak->chunks_[i].id_;

			// write entry [chunk,samples,id]
			write_int32(&stsc_table[stsc_entries].chunk_, 1);
			write_int32(&stsc_table[stsc_entries].samples_, samples);
			write_int32(&stsc_table[stsc_entries].id_, id);
			++stsc_entries;
			if (i != chunk_end)
			{
				for (i += 1; i != chunk_end; ++i)
				{
					if (trak->chunks_[i].size_ != samples)
					{
						samples = trak->chunks_[i].size_;
						id = trak->chunks_[i].id_;
						write_int32(&stsc_table[stsc_entries].chunk_, i - chunk_start + 1);
						write_int32(&stsc_table[stsc_entries].samples_, samples);
						write_int32(&stsc_table[stsc_entries].id_, id);
						++stsc_entries;
					}
				}
			}
			write_int32(stsc + 4, stsc_entries);
			{
				unsigned char* stco = trak->mdia_.minf_.stbl_.stco_;
				//        stco_erase(stco, chunk_start);
				//unsigned int entries = read_int32(stco + 4);
				uint32_t* stco_table = (uint32_t*)(stco + 8);
				memmove(stco_table, &stco_table[chunk_start], (chunk_end - chunk_start) * sizeof(uint32_t));
				write_int32(stco + 4, chunk_end - chunk_start);

				// patch first chunk with correct sample offset
				//        uint32_t* stco_table = (uint32_t*)(stco + 8);
				write_int32(stco_table, trak->samples_[start].pos_);
			}
		}
	}

	// process sync samples:
	if (trak->mdia_.minf_.stbl_.stss_)
	{
		unsigned char* stss = trak->mdia_.minf_.stbl_.stss_;
		unsigned int entries = read_int32(stss + 4);
		uint32_t* table = (uint32_t*)(stss + 8);
		unsigned int stss_start;
		unsigned int i = 0;
		for (i = 0; i != entries; ++i)
		{
			if ((unsigned int)read_int32(&table[i]) >= start + 1)
			{
				break;
			}
		}
		stss_start = i;
		for (; i != entries; ++i)
		{
			unsigned int sync_sample = read_int32(&table[i]);
			if (sync_sample >= end + 1)
			{
				break;
			}
			write_int32(&table[i - stss_start], sync_sample - start);
		}
		//    memmove(table, table + stss_start, (i - stss_start) * sizeof(uint32_t));
		write_int32(stss + 4, i - stss_start);
	}

	// process sample sizes
	{
		unsigned char* stsz = trak->mdia_.minf_.stbl_.stsz_;
		if (stsz_get_sample_size(stsz) == 0)
		{
			uint32_t* table = (uint32_t*)(stsz + 12);
			memmove(table, &table[start], (end - start) * sizeof(uint32_t));
			write_int32(stsz + 8, end - start);
		}
	}
}

int moov_parse(struct moov_t* moov, unsigned char* buffer, unsigned int size)
{
	struct atom_t leaf_atom;
	unsigned char* buffer_start = buffer;
	moov->start_ = buffer;

	while (buffer < buffer_start + size)
	{
		buffer = atom_read_header(buffer, &leaf_atom);

		atom_print(&leaf_atom);

		if (atom_is(&leaf_atom, "cmov"))
		{
			return 0;
		}
		else if (atom_is(&leaf_atom, "mvhd"))
		{
			moov->mvhd_ = buffer;
		}
		else if (atom_is(&leaf_atom, "trak"))
		{
			if (moov->tracks_ == MAX_TRACKS)
				return 0;
			else
			{
				struct trak_t* trak = &moov->traks_[moov->tracks_];
				trak_init(trak);
				trak_parse(trak, buffer, leaf_atom.size_ - ATOM_PREAMBLE_SIZE);
				++moov->tracks_;
			}
		}
		buffer = atom_skip(&leaf_atom);
	}

	// build the indexing tables
	{
		unsigned int i;
		for (i = 0; i != moov->tracks_; ++i)
		{
			trak_build_index(&moov->traks_[i]);
		}
	}

	return 1;
}

void stco_shift_offsets(unsigned char* stco, int offset)
{
	unsigned int entries = read_int32(stco + 4);
	unsigned int* table = (unsigned int*)(stco + 8);
	unsigned int i;
	for (i = 0; i != entries; ++i)
	{
		write_int32(&table[i], (read_int32(&table[i]) + offset));
	}
}

void trak_shift_offsets(struct trak_t* trak, int offset)
{
	unsigned char* stco = trak->mdia_.minf_.stbl_.stco_;
	stco_shift_offsets(stco, offset);
}

void moov_shift_offsets(struct moov_t* moov, int offset)
{
	unsigned int i;
	for (i = 0; i != moov->tracks_; ++i)
	{
		trak_shift_offsets(&moov->traks_[i], offset);
	}
}

long mvhd_get_time_scale(unsigned char* mvhd)
{
	int version = read_char(mvhd);
	unsigned char* p = mvhd + (version == 0 ? 12 : 20);
	return read_int32(p);
}

void mvhd_set_duration(unsigned char* mvhd, long duration)
{
	int version = read_char(mvhd);
	if (version == 0)
	{
		write_int32(mvhd + 16, duration);
	}
	else
	{
		perror("mvhd_set_duration");
	}
}

long mdhd_get_time_scale(unsigned char* mdhd)
{
	return read_int32(mdhd + 12);
}

void mdhd_set_duration(unsigned char* mdhd, unsigned int duration)
{
	write_int32(mdhd + 16, duration);
}

void tkhd_set_duration(unsigned char* tkhd, unsigned int duration)
{
	int version = read_char(tkhd);
	if (version == 0)
	{
		write_int32(tkhd + 20, duration);
	}
	else
	{
		perror("tkhd_set_duration");
	}
}

unsigned int stss_get_entries(unsigned char const* stss)
{
	return read_int32(stss + 4);
}

unsigned int stss_get_sample(unsigned char const* stss, unsigned int idx)
{
	unsigned char const* p = (stss + 8) + (idx * 4);
	return read_int32(p);
}

unsigned int stss_get_nearest_keyframe(unsigned char const* stss, unsigned int sample)
{
	// 在样本数量之前扫描同步样本查找关键帧;
	// scan the sync samples to find the key frame that precedes the sample number
	unsigned int i = 0;
	unsigned int entries = stss_get_entries(stss);
	unsigned int table_sample = 0;
	for (i = 0; i != entries; ++i)
	{
		table_sample = stss_get_sample(stss, i);
		if (table_sample >= sample)
		{
			break;
		}
	}
	if (table_sample == sample)
	{
		return table_sample;
	}

	return stss_get_sample(stss, i - 1);
}

unsigned int stbl_get_nearest_keyframe(struct stbl_t const* stbl, unsigned int sample)
{
	// If the sync atom is not present, all samples are implicit sync samples.
	if (!stbl->stss_)
	{
		// 对于音频, 没有关键帧则返回当前;
		return sample;
	}

	return stss_get_nearest_keyframe(stbl->stss_, sample);
}

unsigned int moov_seek(unsigned char* moov_data, unsigned int size,
	float start_time,
	float end_time,
	unsigned char* &mdat_start,
	unsigned int& mdat_size,
	unsigned int offset)
{
	struct moov_t* moov = (moov_t *)malloc(sizeof(struct moov_t));
	moov_init(moov);
	if (!moov_parse(moov, moov_data, size))
	{
		moov_exit(moov);
		free(moov);
		return 0;
	}

	{
		long moov_time_scale = mvhd_get_time_scale(moov->mvhd_);
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
			struct trak_t* trak = &moov->traks_[i];
			struct stbl_t* stbl = &trak->mdia_.minf_.stbl_;
			long trak_time_scale = mdhd_get_time_scale(trak->mdia_.mdhd_);
			float moov_to_trak_time = (float)trak_time_scale / (float)moov_time_scale;
			float trak_to_moov_time = (float)moov_time_scale / (float)trak_time_scale;

			{
				start = stts_get_sample(stbl->stts_, (unsigned int)(start * moov_to_trak_time));
				start = stbl_get_nearest_keyframe(stbl, start + 1) - 1;
				trak_sample_start[i] = start;
				start = (unsigned int)(stts_get_time(stbl->stts_, start) * trak_to_moov_time);
				printf("track %d : frame_start=%u\n", i, start);
			}

			{
				int iEndPos = stts_get_sample(stbl->stts_, (unsigned int)(end * moov_to_trak_time));
				trak_sample_end[i] = iEndPos;
				printf("track %d : frame_end=%u\n", i, iEndPos);

				/*end = stts_get_sample(stbl->stts_, (unsigned int)(end * moov_to_trak_time));
				end = stbl_get_nearest_keyframe(stbl, end + 1) - 1;
				trak_sample_end[i] = end;
				end = (unsigned int)(stts_get_time(stbl->stts_, end) * trak_to_moov_time);
				printf("track %d : frame_end=%u\n", i, end);*/
			}
		}

		for (unsigned int i = 0; i != moov->tracks_; ++i)
		{
			struct trak_t* trak = &moov->traks_[i];
			struct stbl_t* stbl = &trak->mdia_.minf_.stbl_;
			unsigned int start_sample = trak_sample_start[i];
			unsigned int end_sample = trak_sample_end[i];
			//unsigned int end_sample = trak->samples_size_;

			if (start_sample >= end_sample)
			{
				// 若计算出错退出;
				return 0;
			}
			trak_write_index(trak, start_sample, end_sample);
			{
				int iFirstIndex = 0;
				int iLastIndex = trak->samples_size_-1;
				unsigned beginskip = trak->samples_[start_sample].pos_ - trak->samples_[iFirstIndex].pos_;
				//unsigned endskip = 0; 
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

			{
				// fixup trak (duration)
				unsigned int trak_duration = stts_get_duration(stbl->stts_);
				long trak_time_scale = mdhd_get_time_scale(trak->mdia_.mdhd_);
				float trak_to_moov_time = (float)moov_time_scale / (float)trak_time_scale;
				unsigned int duration = (long)((float)trak_duration * trak_to_moov_time);
				mdhd_set_duration(trak->mdia_.mdhd_, trak_duration);
				tkhd_set_duration(trak->tkhd_, duration);
				printf("trak: new_duration=%d\n", duration);

				if (duration > moov_duration)
				{
					moov_duration = duration;
				}
			}

			printf("stco.size=%d, ", read_int32(stbl->stco_ + 4));
			printf("stts.size=%d samples=%d\n", read_int32(stbl->stts_ + 4), stts_get_samples(stbl->stts_));
			printf("stsz.size=%d\n", read_int32(stbl->stsz_ + 8));
			printf("stsc.samples=%d\n", stsc_get_samples(stbl->stsc_));
		}
		mvhd_set_duration(moov->mvhd_, moov_duration);

		offset -= bytes_to_beginskip;

		printf("shifting offsets by %d\n", offset);
		moov_shift_offsets(moov, offset);

		mdat_start += bytes_to_beginskip;
		mdat_size -= (bytes_to_beginskip + bytes_to_endskip);
	}

	moov_exit(moov);
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
		pData = atom_read_header(pData, &tempAtom);
		if (atom_is(&tempAtom, "moov"))
		{
			moov_data = pData;
			size = tempAtom.size_ - ATOM_PREAMBLE_SIZE;
		}
		else if (atom_is(&tempAtom, "mdat"))
		{
			mdat_start = (unsigned char*)pData;
			mdat_size = tempAtom.size_;
		}
		pData += tempAtom.size_ - ATOM_PREAMBLE_SIZE;
	}

	moov_seek(moov_data, size, start_time, end_time, mdat_start, mdat_size, offset);

	{
		pData = pBegin;
		while (pData < pEnd)
		{
			atom_t tempAtom;
			pData = atom_read_header(pData, &tempAtom);
			if (atom_is(&tempAtom, "moov"))
			{
				// moov;
				strOutBuff.append(std::string((char*)(moov_data - ATOM_PREAMBLE_SIZE), size + ATOM_PREAMBLE_SIZE));
				iOutSize += size + ATOM_PREAMBLE_SIZE;
			}
			else if (atom_is(&tempAtom, "mdat"))
			{
				// mdat;
				unsigned char mdatHeader[ATOM_PREAMBLE_SIZE] = { 0 };
				write_int32(&mdatHeader[0], mdat_size + sizeof(mdatHeader));
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
