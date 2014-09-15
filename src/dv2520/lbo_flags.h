#ifndef LBO_FLAGS_H
#define LBO_FLAGS_H

namespace lbo {
    // Internal representation of User flags; not for the User.
    enum Flags {
        Flags_FACE_POS = 0x01,
        Flags_FACE_NOR = 0x02,
        Flags_FACE_TEX = 0x04,
        Flags_POS      = 0x08,
        Flags_TEX      = 0x10,
        Flags_NOR      = 0x20,
        Flags_GEN_IDX  = 0x40
    };
}

#endif // LBO_FLAGS_H
