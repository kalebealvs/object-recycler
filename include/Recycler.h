#include <memory>
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
    std::vector<std::unique_ptr<T>> _store; // should not be a vector
    static std::shared_ptr<Recycler<T>> _instance;
};

template<typename T>
std::shared_ptr<Recycler<T>> Recycler<T>::_instance{nullptr}; 

template<typename T>
inline auto Recycler<T>::getInstance () -> std::shared_ptr<Recycler<T>> {
    if (!_instance)
        _instance = std::shared_ptr<Recycler<T>> (new Recycler<T>());
    
    return _instance;
}

template<typename T>
template<typename ...Args>
inline auto Recycler<T>::recycle(const Args& ...args) -> std::unique_ptr<T> {
    if(_store.empty())
        return std::make_unique<T>(args...);

    auto recycled = std::move(_store.back());
    _store.pop_back();
    return recycled;
}

template<typename T>
inline auto Recycler<T>::trash(std::unique_ptr<T> item) -> void {
    if (nullptr == item)
        return;

    _store.push_back(std::move(item));
}