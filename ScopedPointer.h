#ifndef SCOPEDPOINTER_H
#define SCOPEDPOINTER_H

#include <iostream>

template<typename DataType>
struct DefaultDeleter
{
	void operator()(DataType*& ptr) const {
		delete ptr;
		ptr = nullptr;
	}
};

template<typename DataType, typename Deleter = DefaultDeleter<DataType>>
class ScopedPointer
{
public:
	explicit ScopedPointer(DataType* pointer = nullptr);
	ScopedPointer(ScopedPointer&& other) noexcept;
	ScopedPointer& operator=(ScopedPointer&& other) noexcept;
	ScopedPointer(const ScopedPointer& other) = delete;
	ScopedPointer& operator=(const ScopedPointer& other) = delete;
	~ScopedPointer();


	DataType operator*();
    DataType* operator->();

	DataType* get();
	DataType* release();

private:
	DataType* m_pointerToData;
	Deleter m_deleter;
};



template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>::ScopedPointer(DataType* pointer)
	: m_pointerToData(pointer)
{
	std::cout << "ScopedPointer()\n";
}

template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>::ScopedPointer(ScopedPointer&& other) noexcept
	: m_pointerToData(std::move(other.release()))
	, m_deleter(std::move(other.m_deleter))
{
	std::cout << "ScopedPointer(ScopedPointer&& other)\n";
}

template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>& ScopedPointer<DataType, Deleter>::operator=(ScopedPointer&& other) noexcept
{
	std::cout << "operator=(ScopedPointer&& other)";
	if (this == &other) {
		return *this;
	}

	m_pointerToData = std::move(other.release());
	m_deleter = std::move(other.m_deleter);
	return *this;
}

template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>::~ScopedPointer()
{
	std::cout << "~ScopedPointer() started. m_pointerToData = " << m_pointerToData << "\n";
	m_deleter(m_pointerToData);
	std::cout << "~ScopedPointer() finished. m_pointerToData = " << m_pointerToData << "\n";
}

template<typename DataType, typename Deleter>
inline DataType ScopedPointer<DataType, Deleter>::operator*()
{
	return *m_pointerToData;
}

template<typename DataType, typename Deleter>
inline DataType* ScopedPointer<DataType, Deleter>::operator->()
{
	return m_pointerToData;
}

template<typename DataType, typename Deleter>
inline DataType* ScopedPointer<DataType, Deleter>::get()
{
	return m_pointerToData;
}

template<typename DataType, typename Deleter>
inline DataType* ScopedPointer<DataType, Deleter>::release()
{
	auto tmp = m_pointerToData;
	m_pointerToData = nullptr;
	return tmp;
}

#endif // !SCOPEDPOINTER_H