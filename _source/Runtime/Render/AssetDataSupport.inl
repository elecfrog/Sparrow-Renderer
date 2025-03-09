/*
 * @date   2023-04-09
 * @author dudu
 * @brief  ${Cereal Serialization Data Types Supported}
 */
#pragma once

// ----------- GLM Atom Data Types -------------------
#include <glm/glm.hpp>

namespace glm {
    template<class Archive, class T>
    inline void serialize(Archive &archive, vec<2, T, defaultp> &v) {
        archive(cereal::make_nvp("x", v.x),
                cereal::make_nvp("y", v.y));
    }

    template<class Archive, class T>
    inline void serialize(Archive &archive, vec<3, T, defaultp> &v) {
        archive(cereal::make_nvp("x", v.x),
                cereal::make_nvp("y", v.y),
                cereal::make_nvp("z", v.z));
    }

    template<class Archive, class T>
    inline void serialize(Archive &archive, vec<4, T, defaultp> &v) {
        archive(cereal::make_nvp("x", v.x),
                cereal::make_nvp("y", v.y),
                cereal::make_nvp("z", v.z),
                cereal::make_nvp("w", v.w));
    }

    // glm matrices serialization
    template<class Archive, class T>
    inline void serialize(Archive &archive, mat<2, 2, T, defaultp> &m) { archive(m[0], m[1]); }

    template<class Archive, class T>
    inline void serialize(Archive &archive, mat<3, 3, T, defaultp> &m) { archive(m[0], m[1], m[2]); }

    template<class Archive, class T>
    inline void serialize(Archive &archive, mat<4, 4, T, defaultp> &m) { archive(m[0], m[1], m[2], m[3]); }

    template<class Archive, class T>
    inline void serialize(Archive &archive, qua<T, defaultp> &q) {
        archive(cereal::make_nvp("x", q.x),
                cereal::make_nvp("y", q.y),
                cereal::make_nvp("z", q.z),
                cereal::make_nvp("w", q.w));
    }
}// namespace glm


// ----------- ENUM Type Support -------------------
namespace cereal {

} // namespace cereal