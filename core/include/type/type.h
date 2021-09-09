#pragma once

namespace seam::type {
    enum class BuiltIn {
        None, // No assigned type
        Bool, // Boolean (1 byte)
        Char, // Character (1 byte)
        String, // First class string (variable size, probably pointer due to heap alloc)
        i8, // 1 byte (8 bit) signed integer
        i16, // 2 byte (16 bit) signed integer
        i32, // 4 byte (32 bit) signed integer
        i64, // 8 byte (64 bit) signed integer
        u8, // 1 byte (8 bit) unsigned integer
        u16, // 2 byte (16 bit) unsigned integer
        u32, // 4 byte (32 bit) unsigned integer
        u64, // 8 byte (64 bit) unsigned integer
        f32, // 4 byte (32 bit) float
        f64, // 8 byte (64 bit) float

    }
}