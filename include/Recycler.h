#include <memory>

template<typename T>
class Recycler {
public:
    static auto getInstance() -> std::shared_ptr<Recycler<T>>;
    
    template<typename ...Args>
    auto recycle(const Args& ...args) -> std::unique_ptr<T>;

private:
    Recycler () = default;
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
    return std::make_unique<T>(args...);
}