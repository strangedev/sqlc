//
// Created by sdv on 1/25/20.
//

#ifndef SQLC_UIDREALM_T_H
#define SQLC_UIDREALM_T_H

#include <optional>
#include <map>
#include <shared_mutex>

namespace sqlc
{
    enum UIDError
    {
        AliasIsAlreadyAssociated
    };

    template<typename T>
    class UIDRealm
    {
    public:
        UIDRealm():
            alias_to_uid(std::map<T, uid_t>()),
            uid_to_alias(std::map<uid_t, T>()),
            next_uid(0)
        { }
        ~UIDRealm() = default;
        uid_t associate_uid(T alias)
        {
            std::lock_guard exclusive(lock);
            if(is_associated(alias)) throw AliasIsAlreadyAssociated;
            auto uid = next_uid++;
            alias_to_uid.insert({alias, uid});
            uid_to_alias.insert({uid, alias});
            return uid;
        }
        bool is_associated(T alias)
        {
            lock.lock_shared();
            auto entry = alias_to_uid.find(alias);
            auto alias_is_associated = entry != alias_to_uid.end();
            lock.unlock_shared();

            return alias_is_associated;
        }
        std::optional<uid_t> lookup_uid(T alias)
        {
            std::optional<uid_t> uid;

            lock.lock_shared();
            auto entry = alias_to_uid.find(alias);
            if(entry != alias_to_uid.end())
                uid = alias_to_uid.at(alias);
            lock.unlock_shared();

            return uid;
        }
        std::optional<T> lookup_alias(uid_t uid)
        {
            std::optional<T> alias;

            lock.lock_shared();
            auto entry = uid_to_alias.find(uid);
            if(entry != uid_to_alias.end())
                alias = uid_to_alias.at(uid);
            lock.unlock_shared();

            return alias;
        }

    private:
        std::map<T, uid_t> alias_to_uid;
        std::map<uid_t, T> uid_to_alias;
        std::shared_mutex lock;
        uid_t next_uid;
    };

} // namespace sqlc

#endif //SQLC_UIDREALM_T_H
