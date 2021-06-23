#include <memory>
#include <mutex>
#include <vector>

template<typename T>
class Recycler {
public:
    static auto getInstance() -> std::shared_ptr<Recycler<T>>;
    
    template<typename ...Args>
    auto recycle(const Args& ...args) -> std::unique_ptr<T>;
    auto trash(std::unique_ptr<T> item) -> void;

private:
    Recycler () = default;
    
    std::mutex _store_mutex;
    std::vector<std::unique_ptr<T>> _store; // should not be a vector
    
    static std::mutex _instance_mutex;
    static std::shared_ptr<Recycler<T>> _instance;
};

template<typename T>
std::shared_ptr<Recycler<T>> Recycler<T>::_instance{nullptr}; 

template<typename T>
std::mutex Recycler<T>::_instance_mutex;

template<typename T>
inline auto Recycler<T>::getInstance () -> std::shared_ptr<Recycler<T>> {
    std::scoped_lock lock (_instance_mutex);
    if (!_instance)
        _instance = std::shared_ptr<Recycler<T>> (new Recycler<T>());
    
    return _instance;
}

template<typename T>
template<typename ...Args>
inline auto Recycler<T>::recycle(const Args& ...args) -> std::unique_ptr<T> {
    std::unique_lock lock (_store_mutex);
    if(_store.empty())
        return std::make_unique<T>(args...);

    auto recycled = std::move(_store.back());
    _store.pop_back();
    lock.unlock ();
    return recycled;
}

template<typename T>
inline auto Recycler<T>::trash(std::unique_ptr<T> item) -> void {
    if (nullptr == item)
        return;

    std::scoped_lock lock (_store_mutex);
    _store.push_back(std::move(item));
}