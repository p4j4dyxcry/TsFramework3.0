#pragma once

#pragma pack(1)

// ファイル先頭8バイト
struct png_signature
{
    unsigned char data[8];

    bool is_png()const
    {
        static const unsigned char png_sig[8] = { 137,80,78,71,13,10,26,10 };

        for (int i = 0; i < 8; ++i)
            if (data[i] != png_sig[i])
                return false;
        return true;;
    }
};

enum class png_color_type : unsigned char
{
    use_pallet = 0x01,
    use_rgb    = 0x20,
    use_alpha  = 0x40,
};

inline unsigned operator &(png_color_type l, png_color_type r)
{
    return static_cast<unsigned>(l) & static_cast<unsigned>(r);
}

inline unsigned operator |(png_color_type l, png_color_type r)
{
    return static_cast<unsigned>(l) | static_cast<unsigned>(r);
}

struct png_chunk_header
{
    unsigned length;    
    unsigned char chunk_type[4];

    bool is(const char* element)
    {
        for (int i = 0; i < 4; ++i)
            if (chunk_type[i] != element[i])
                return false;
        return true;

    }
};

struct png_ihdr_chunk
{
    unsigned length;     // 13
    unsigned chunk_type; // IHDR

    unsigned width;
    unsigned height;
    unsigned char bit_depth;
    png_color_type color_type;
    unsigned char compression_method; // 0
    unsigned char filter_method;      // 0
    unsigned char intelace_method;    

    bool is_color() const
    {
        return color_type & png_color_type::use_rgb;
    }

    bool is_gray_scale() const
    {
        return !is_color();
    }

    bool use_pallet()const
    {
        return color_type & png_color_type::use_pallet;
    }

    bool use_alpha()const
    {
        return color_type & png_color_type::use_alpha;
    }


    size_t num_element_of_pixel()
    {
        size_t size = 1;
        if (is_color()) size += 3;
        if (use_alpha()) size += 1;
        return size;
    }
};

struct png_palette
{
    unsigned char data[1024];
    unsigned length;
};

namespace TS
{
    class PngLoader
    {
    public:

        bool IsPng(const char * filename)
        {
            ManagedArray<unsigned char> binary = ReadBinary(filename);

            if (binary == nullptr)
                return false;

            return BinaryReader(binary).Read<png_signature>().is_png();
        }

        void load(const char * filename)
        {
            ManagedArray<unsigned char> binary = ReadBinary(filename);
            BinaryReader binary_reader(binary);

            png_signature signature = binary_reader.Read<png_signature>();
            png_ihdr_chunk ihdr_chunk = binary_reader.Read<png_ihdr_chunk>();
            Collection<unsigned char> color_buffer;
            png_palette palette;

            while(true)
            {
                png_chunk_header chunk_header = binary_reader.Read<png_chunk_header>();

                if(chunk_header.is("PLTE"))
                {
                    palette.length = chunk_header.length;

                    for(unsigned i = 0 ; i < palette.length ; ++i)
                    {
                        palette.data[i * 4 + 0] = binary_reader.Read<unsigned char>();
                        palette.data[i * 4 + 1] = binary_reader.Read<unsigned char>();
                        palette.data[i * 4 + 2] = binary_reader.Read<unsigned char>();
                        palette.data[i * 4 + 3] = 255;
                    }
                }
                else if (chunk_header.is("tRNS"))
                {
                    if(ihdr_chunk.use_pallet())
                    {
                        for (unsigned i = 0; i < chunk_header.length; ++i)
                            palette.data[i * 4 + 3] = binary_reader.Read<unsigned char>();
                    }
                    else
                    {
                        // TODO
                    }
                }
                else if (chunk_header.is("IDAT"))
                {
                    const unsigned buffer_start = color_buffer.Length();
                    color_buffer.Resize(color_buffer.Length() + chunk_header.length);
                    binary_reader.ReadArray<unsigned char>(&color_buffer[buffer_start], chunk_header.length);
                }

                // skip crc
                binary_reader.Skip(1);

            }
        }
    };
}


#pragma pack()