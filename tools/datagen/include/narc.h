#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <narc/narc.h>
#include <vector>

namespace fs = std::filesystem;

struct Narc {
    vfs_pack_ctx *m_packctx = nullptr;

    Narc()
        : m_packctx(narc_pack_start())
    {
    }

    template <typename T>
    Narc *operator<<(T &vfile)
    {
        narc_pack_file_copy(
            this->m_packctx,
            const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(&vfile)),
            sizeof(vfile));
        return this;
    }

    template <typename T>
    Narc *operator<<(std::vector<T> &vtable)
    {
        narc_pack_file_copy(
            this->m_packctx,
            reinterpret_cast<unsigned char *>(vtable.data()),
            sizeof(vtable[0]) * vtable.size());
        return this;
    }

    void pack(const fs::path &outp)
    {
        narc *narc = narc_pack(this->m_packctx);
        std::ofstream ofs(outp);
        ofs.write(reinterpret_cast<char *>(narc), narc->size);

        std::free(narc);
    }
};
