﻿#include "sstd_virtual_basic.hpp"
#include <cassert>
#include <unordered_map>
#include <shared_mutex>

const sstd_virtual_basic::sstd_type_index_t & sstd_virtual_basic::sstd_get_type_index() const {
    using sstd_this_type_ = std::remove_cv_t<
        std::remove_reference_t< decltype(*this) > >;
    return ::sstd_get_type_index<sstd_this_type_>();
}

void * sstd_virtual_basic::sstd_get_this_pointer() const {
    return nullptr;
}

const sstd_virtual_basic::items_map_t * sstd_virtual_basic::sstd_get_super_objects_map() const {
    return nullptr;
}

sstd_virtual_basic::sstd_virtual_basic() noexcept {
}

sstd_virtual_basic::~sstd_virtual_basic() {
    destory_objects_in_this();
    delete mmm_named_objects;
    delete mmm_objects_in_this;
    delete mmm_mutex.load();
}

void * sstd_virtual_basic::sstd_find_object(const sstd_type_index_t & k) const {
    auto varMap = sstd_get_super_objects_map();
    auto varCastPos = varMap->find(k);
    if (varCastPos == varMap->end()) {
        return nullptr;
    }
    return (varCastPos->second)( this->sstd_get_this_pointer() );
}

sstd_virtual_basic::maped_named_objects_t * sstd_virtual_basic::get_named_objects() noexcept {
    if (mmm_named_objects == nullptr) {
        mmm_named_objects = new maped_named_objects_t_t;
    }
    return mmm_named_objects->get_type_data();
}

sstd_virtual_basic::mmm_objects_in_this_t * sstd_virtual_basic::get_objects_list() noexcept {
    if (mmm_objects_in_this == nullptr) {
        mmm_objects_in_this = new mmm_objects_in_this_t_t;
    }
    return mmm_objects_in_this->get_type_data();
}

void sstd_virtual_basic::destory_objects_in_this() noexcept {
    if (mmm_objects_in_this) {
        auto p = mmm_objects_in_this->get_type_data();
        while (p->empty() == false) {
            p->pop_front();
        }
    }
}

void sstd_virtual_basic::insert_object(std::unique_ptr< private_object_t  > && arg) noexcept {
    get_objects_list()->push_front(std::move(arg));
}

void sstd_virtual_basic::insert_named_object(const std::string_view &name, std::unique_ptr< private_object_t  > && arg) noexcept {
    get_named_objects()->emplace(name, arg.get());
    get_objects_list()->push_front(std::move(arg));
}

void * sstd_virtual_basic::sstd_find_named_object(const std::string_view &name) const noexcept {
    auto varMap = const_cast<sstd_virtual_basic *>(this)->get_named_objects();
    auto varPos = varMap->find(name);
    if (varPos == varMap->end()) {
        return nullptr;
    }
    return varPos->second->get_data();
}

void _18_11_18_private::named_object::set_name(const std::string_view & name) {
    *(mmm_name->get_type_data()) = name;
}

std::string_view _18_11_18_private::named_object::get_name() const {
    return *(mmm_name->get_type_data());
}

_18_11_18_private::named_object::named_object() noexcept {
    mmm_name = new string_type;
}

_18_11_18_private::named_object::~named_object() noexcept {
    delete mmm_name;
}

_18_11_18_private::object::~object() noexcept {
}

_18_11_18_private::object::object() noexcept {
}

std::shared_ptr< std::recursive_mutex > sstd_virtual_basic::sstd_get_class_mutex() const noexcept {
    auto varAns = mmm_mutex.load();
    if (varAns) {
        return *(varAns->get_type_data());
    }

    {
        auto varData = new mutex_t_t;
        mutex_t_t * varTmp{ nullptr };
        const bool varSetValue = mmm_mutex.compare_exchange_strong(varTmp, varData);
        if (varSetValue) {
            varAns = varData;
            (*(varData->get_type_data())) = std::make_shared< std::recursive_mutex >();
            assert(varAns == mmm_mutex.load());
        } else {
            assert(varTmp==mmm_mutex.load());
            assert(varData!=varTmp);
            delete varData;
            varAns = varTmp;
            assert(varAns == nullptr);
        }
    }

    return *(varAns->get_type_data());
}








