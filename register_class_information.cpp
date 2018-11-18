#include "register_class_information.hpp"
#include <unordered_map>
#include <shared_mutex>

namespace _18_11_18_private {

    namespace {
        namespace this_cpp_file {

            class type_index_hash {
            public:
                std::size_t operator()(const ExportRegisterClassInformation::type_index & arg) const {
                    return arg.first;
                }
            };

            class class_information {
            public:
                ExportRegisterClassInformation::static_cast_map up_cast_map;
                class_information() {
                }
                class_information(ExportRegisterClassInformation::static_cast_map && arg) :
                    up_cast_map(std::move(arg)) {
                }
            };

            class type_index_hash_map {
                using hash_map = std::unordered_map<
                    ExportRegisterClassInformation::type_index,
                    class_information,
                    type_index_hash
                >;
                hash_map mmm_hash_map;
                std::shared_mutex mmm_mutex;
            public:
                const class_information * insert(
                    const ExportRegisterClassInformation::type_index & key,
                    ExportRegisterClassInformation::static_cast_map && up_cast_map) {
                        {/*try to find ...*/
                            std::shared_lock varReadLock{ mmm_mutex };
                            auto varPos = std::as_const(mmm_hash_map).find(key);
                            if (varPos != mmm_hash_map.end()) {
                                return &(varPos->second);
                            }
                        }
                        std::unique_lock varWriteLock{ mmm_mutex };
                        {/*try to find again ...*/
                            auto varPos = std::as_const(mmm_hash_map).find(key);
                            if (varPos != mmm_hash_map.end()) {
                                return &(varPos->second);
                            }
                        }
                        /*insert the data ...*/
                        auto varPos = mmm_hash_map.emplace(key, std::move(up_cast_map));
                        return &(varPos.first->second);
                }

                const class_information * find(
                        const ExportRegisterClassInformation::type_index & key) {
                    std::shared_lock varReadLock{ mmm_mutex };
                    auto varPos = std::as_const(mmm_hash_map).find(key);
                    if (varPos != mmm_hash_map.end()) {
                        return &(varPos->second);
                    }
                    return nullptr;
                }

            }/*endl class */;

            static inline type_index_hash_map * get_type_index_hash_map() {
                /*never delete ...*/
                static type_index_hash_map * varAns{ new type_index_hash_map };
                return varAns;
            }

        }/**/
    }/**/

    const ExportRegisterClassInformation::static_cast_map * ExportRegisterClassInformation::register_up_cast_map(
        const type_index & argIndex,
        static_cast_map && argMap) {
        auto varMap = this_cpp_file::get_type_index_hash_map();
        return &(varMap->insert(argIndex, std::move(argMap))->up_cast_map);
    }

    const ExportRegisterClassInformation::static_cast_map * ExportRegisterClassInformation::find_up_cast_map(
            const type_index & argIndex){
        auto varMap = this_cpp_file::get_type_index_hash_map();
        auto varAns = varMap->find( argIndex );
        if(varAns){
            return &(varAns->up_cast_map);
        }
        return nullptr;
    }

}/*namespace _18_11_18_private*/















