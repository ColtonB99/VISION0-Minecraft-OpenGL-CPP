#ifndef ICHUNK_H_INCLUDED
#define ICHUNK_H_INCLUDED

struct IChunk {
    virtual ~IChunk() = default;

    //virtual ChunkBlock getBlock(int x, int y, int z) const = 0;
    //virtual void setBlock(int x, int y, int z, ChunkBlock block) = 0; BOTH COMMENTED OUT UNTIL IMPLEMENTATION
};

#endif // ICHUNK_H_INCLUDED