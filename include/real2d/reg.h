#pragma once
#include <map>
#include <string>

namespace Real2D {
    class Block;
    template<typename T>
    class Registry {
    private:
        std::map<std::string, T> id2entry;
        std::map<T, std::string> entry2id;
        std::map<int, T> rawId2entry;
        std::map<T, int> entry2rawId;
        int nextId;
        T* default_entry;
    public:
        Registry(T* _defaultEntry = nullptr) :
            nextId(0),
            default_entry(_defaultEntry) {}

        T set(int rawId, std::string id, T entry) {
            if (rawId > nextId) {
                nextId = rawId;
            }
            id2entry[id] = entry;
            entry2id[entry] = id;
            rawId2entry[rawId] = entry;
            entry2rawId[entry] = rawId;
            return entry;
        }

        T add(std::string id, T entry) {
            set(nextId, id, entry);
            ++nextId;
            return entry;
        }

        int get(T entry) {
            if (entry2rawId.count(entry) < 1) {
                return 0;
            }
            return entry2rawId[entry];
        }

        std::string getSID(T entry) {
            if (entry2id.count(entry) < 1) {
                return entry2id[*default_entry];
            }
            return entry2id[entry];
        }

        std::string getSID(int rawId) {
            if (rawId2entry.count(rawId) < 1) {
                return entry2id[*default_entry];
            }
            return getSID(rawId2entry[rawId]);
        }

        T get(std::string id) {
            if (id2entry.count(id) < 1) {
                // may throw exception
                // won't "fix"
                return *default_entry;
            }
            return id2entry[id];
        }

        T get(int rawId) {
            if (rawId2entry.count(rawId) < 1) {
                // may throw exception
                // won't "fix"
                return *default_entry;
            }
            return rawId2entry[rawId];
        }

        std::map<T, int> rawIds() {
            return entry2rawId;
        }

        int size() {
            return (int)id2entry.size();
        }
    };

    struct Registries {
        static Registry<Block*>* BLOCK;

        template<typename T, typename R>
        R reg(Registry<T> registry, std::string id, R entry) {
            return registry.add(id, entry);
        }
    };
}
