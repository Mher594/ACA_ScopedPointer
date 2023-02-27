#ifndef SCOPEDPOINTER_H
#define SCOPEDPOINTER_H

#include <utility>

namespace ACA
{

template<typename DataType>
struct DefaultDeleter
{
	DefaultDeleter() = default;
	template <class OtherDataType>
	DefaultDeleter(const DefaultDeleter<OtherDataType>& other) noexcept
	{
	}

	void operator()(DataType*& ptr) const 
	{
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
	template<class OtherDataType, class OtherDeleter>
	ScopedPointer(ScopedPointer<OtherDataType, OtherDeleter>&& other) noexcept
		: m_pointerToData(std::move(other.release()))
		, m_deleter(std::move(other.getDeleter()))
	{
	}

	ScopedPointer& operator=(ScopedPointer&& other) noexcept;

	ScopedPointer(const ScopedPointer& other) = delete;
	ScopedPointer& operator=(const ScopedPointer& other) = delete;
	~ScopedPointer();


	DataType& operator*();
    DataType* operator->();

	DataType* get();
	DataType* release();
    void reset(DataType* other);

	[[nodiscard]] Deleter getDeleter() const;

private:
	DataType* m_pointerToData;
	Deleter m_deleter;
};



template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>::ScopedPointer(DataType* pointer)
	: m_pointerToData(pointer)
{
}

template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>::ScopedPointer(ScopedPointer&& other) noexcept
	: m_pointerToData(std::move(other.release()))
	, m_deleter(std::move(other.getDeleter()))
{
}

template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>& ScopedPointer<DataType, Deleter>::operator=(ScopedPointer&& other) noexcept
{
	if (this == &other) 
	{
		return *this;
	}

	m_pointerToData = std::move(other.release());
	m_deleter = std::move(other.m_deleter);
	return *this;
}

template<typename DataType, typename Deleter>
inline ScopedPointer<DataType, Deleter>::~ScopedPointer()
{
	m_deleter(m_pointerToData);
}

template<typename DataType, typename Deleter>
inline DataType& ScopedPointer<DataType, Deleter>::operator*()
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

template<typename DataType, typename Deleter>
inline void ScopedPointer<DataType, Deleter>::reset(DataType* other)
{
	m_deleter(m_pointerToData);
	m_pointerToData = other;
}

template<typename DataType, typename Deleter>
inline Deleter ScopedPointer<DataType, Deleter>::getDeleter() const
{
	return m_deleter;
}

} // namespace ACA

#endif // !SCOPEDPOINTER_H