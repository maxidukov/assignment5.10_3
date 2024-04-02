#include <iostream>


template <typename T>
class my_unique_ptr{
   private:
    T* ptr_;
   public:
    my_unique_ptr(): ptr_ (nullptr){}
    //my_unique_ptr<T>(): ptr_ (nullptr){} //COMPILES WITH -STD=C++17 BUT NOT -STD=C++20
    my_unique_ptr(T* ptr): ptr_(ptr){}
    //my_unique_ptr(T* ptr): ptr_(ptr){}  //COMPILES WITH -STD=C++17 BUT NOT -STD=C++20

    my_unique_ptr (const my_unique_ptr<T>& other) = delete;
    //my_unique_ptr<T> (const my_unique_ptr<T>& other) = delete;  //COMPILES WITH -STD=C++17 BUT NOT -STD=C++20
    my_unique_ptr<T>& operator= (const my_unique_ptr<T>& other) = delete; //REDUNDANT!!

    my_unique_ptr (my_unique_ptr&& other) noexcept: ptr_(nullptr){
    //my_unique_ptr (my_unique_ptr<T>&& other) noexcept: ptr_(nullptr){ //COMPILES WITH -STD=C++17 BUT NOT -STD=C++20
      ptr_ = std::move(other.ptr_);
      other.ptr_ = nullptr;
      //other.swap(*this);
    }
    my_unique_ptr<T>& operator= (const my_unique_ptr<T>&& other) noexcept {
      if (this == other) return *this;
      ptr_ = std::move(other.ptr_);
      other.ptr_ = nullptr;
      //other.swap(*this);
      return *this;
    }
    T& operator* () const{
      return *ptr_;
    }
    T* operator->() const{
      return ptr_;
    }
    T* release() noexcept{
      T* temp = nullptr;
      temp = std::move(ptr_);
      ptr_ = nullptr;
      return temp;
    }

    //void reset(){}
    //T* swap(my_unique_ptr<T>& other) noexcept{}

    ~my_unique_ptr(){
      std::cout<<"Dtor called!\n";
      delete ptr_;
    }
};

//template <typename T>
//class my_unique_ptr<T[]>  {}

//template <class T, class ...Args>
//enable_if_t<!is_array_v<T>, my_unique_ptr<T>>
//my_make_unique(Args&& ...args)

int main() {
  my_unique_ptr<int> muptr(new int(10));
  //my_unique_ptr<int> muptr2(muptr);   //WILL NOT COMPILE, CTOR REMOVED
  my_unique_ptr<int> muptr2(std::move(muptr));     //DTOR CALLED ON muptr
  //my_unique_ptr<int> muptr3 = muptr;  //WILL NOT COMPILE
  my_unique_ptr<int> muptr3 = std::move(muptr2);   //DTOR CALLED ON muptr2
  std::cout << *muptr3 << std::endl;
  int* rawptr = muptr3.release();                  //DTOR CALLED ON muptr3
  std::cout << *rawptr << std::endl;
  delete rawptr;
}
//std::cout BUFFER CONTENT RELEASED, ORDER IS DIFFEREMT FROM THAT IN THE CODE!!!
