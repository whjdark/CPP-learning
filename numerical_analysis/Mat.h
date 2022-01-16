#pragma once

#include <memory>
#include <array>
#include <vector>

#include "error.h"

namespace ht
{

// matrix template type check 
template<typename T>
struct is_valid_T
{
	static constexpr bool value = std::is_default_constructible<T>::value &&
		std::is_nothrow_copy_constructible<T>::value &&
		std::is_nothrow_move_constructible<T>::value &&
		std::is_nothrow_copy_assignable<T>::value &&
		std::is_nothrow_move_assignable<T>::value &&
		std::is_nothrow_destructible<T>::value &&
		!std::is_void<T>::value &&
		!std::is_pointer<T>::value &&
		!std::is_array<T>::value &&
		!std::is_union<T>::value &&
		!std::is_function<T>::value &&
		!std::is_abstract<T>::value;
};

template<typename T, class Allocator = std::allocator<T>>
class Mat
{
private:
	static_assert(is_valid_T<T>::value, "Template type is not a valid type for Mat");
	static_assert(std::is_same<T, typename Allocator::value_type>::value, "value_type and Allocator::value_type must match");

	using AllocType = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
	using AllocTraits = std::allocator_traits<AllocType>;

	using pointer = typename AllocTraits::pointer;
	using const_pointer = typename AllocTraits::const_pointer;
	using ref = T&;
	using const_ref = const T&;

public:
	// default constructor
	Mat() = default;

	// square matrix constructor
	explicit Mat(const size_t _squareSize) : 
		m_row(_squareSize),
		m_col(_squareSize),
		m_size(_squareSize * _squareSize)
	{
		this->create();
	}

	// m x n matrix constructor
	explicit Mat(const size_t _row, const size_t _col) :
		m_row(_row), 
		m_col(_col), 
		m_size(_row * _col)
	{
		this->create();
	}

	// copy constructor 
	// deep copy
	Mat(const Mat<T>& m) :
		m_row(m.row()),
		m_col(m.col()),
		m_size(m.size()),
		m_allocator(m.allocator())
	{
		this->create();
		memcpy_s(m_data, m_size * sizeof(T), m.constDataPtr(), m.m_size * sizeof(T));
	}

	// move constructor 
	Mat(Mat<T>&& m) noexcept :
		m_row(m.row()),
		m_col(m.col()),
		m_size(m.size()),
		m_allocator(m.allocator()),
		m_data(m.dataPtr()) 
	{
		m.dataPtr() == nullptr;
		m.release();
	}

	// copy constructor with 1D-array params
	template<size_t ArraySize>
	Mat(const std::array<T, ArraySize>& array) :
		m_row(1),
		m_col(ArraySize),
		m_size(ArraySize),
	{
		this->create();
		memcpy_s(m_data, m_size * sizeof(T), array.data(), array.size() * sizeof(T));
	}
	
	// copy constructor with 2D-array params
	template<size_t dim0, size_t dim1>
	Mat(const std::array<std::array<T, dim0>, dim1>& array) :
		m_row(dim1),
		m_col(dim0),
		m_size(dim0 * dim1),
	{
		this->create();
		memcpy_s(m_data, m_size * sizeof(T), array.data(), dim0 * dim1 * sizeof(T));
	}

	// copy constructor with 1D-vector params
	Mat(const std::vector<T>& vector) :
		m_row(1),
		m_col(vector.size()),
		m_size(vector.size()),
	{
		this->create();
		memcpy_s(m_data, m_size * sizeof(T), vector.data(), vector.size() * sizeof(T));
	}

	// move constructor with 1D-vector params
	Mat(std::vector<T>&& vector) :
		m_row(1),
		m_col(vector.size()),
		m_size(vector.size()),
		m_data(vector.data())
	{
		vector.data() = nullptr;
	}

	// copy constructor with 2D-vector params
	Mat(const std::vector<std::vector<T>>& vector) :
		m_row(vector.size()),
		m_col(0),
	{
		for (const auto& row : vector)
		{
			if (m_col == 0)
			{
				m_col = row.size();
			}
			else if (row.size() != m_col)
			{
				THROW_INVALID_ARGUMENT_ERROR("All rows of the 2d vector need to have the same number of elements");
			}
		}

		m_size = m_row * m_col;
		this->create();

		pointer currentPosition = m_data;
		for (const auto& row : vector)
		{
			memcpy_s(currentPosition, row.size() * sizeof(T), row.data(), row.size() * sizeof(T));
			currentPosition += m_col;
		}
	}

	// copy constructor with 2D-vector-array params
	template<size_t dim0>
	Mat(const std::vector<std::array<T, dim0>>& vector) :
		m_row(vector.size()),
		m_col(dim0),
		m_size(vector.size() * dim0)
	{
		if (m_size > 0)
		{
			this->create();
			memcpy_s(m_data, m_size * sizeof(T), vector.data(), vector.size() * dim0 * sizeof(T));
		}
	}


	// move constructor with 2D-vector-array params
	template<size_t dim0>
	Mat(std::vector<std::array<T, dim0>>&& vector) :
		m_row(vector.size()),
		m_col(dim0),
		m_size(vector.size() * dim0)
	{
		if (m_size > 0)
		{

		}
	}


	// assignment operator
	// deep copy
	Mat<T>& operator=(const Mat<T>& m)
	{
		if (this == &M)
		{
			return;
		}
		this->release();
		m_row = m.row();
		m_col = m.col();
		m_size = m.size();
		m_allocator = m.allocator();
		this->create();
		memcpy(m_data, m.constDataPtr, m.m_size() * sizeof(T));
	}

	// move assignment operator
	Mat<T>& operator=(Mat<T>&& m) noexcept
	{
		if (this == &M)
		{
			return;
		}
		this->release();
		m_row = m.row();
		m_col = m.col();
		m_size = m.size();
		m_allocator = m.allocator();
		m_data = m.dataPtr();
		m.dataPtr() = nullptr;
		m.release();
	}



	// Destructor
	virtual ~Mat() noexcept
	{
		this->release();
	}

	// opencv-style access Mat::at(i,j) write version
	inline
	ref at(const size_t _row, const size_t _col)
	{
		if (dataPtr == nullptr)
		{
			THROW_RUNTIME_ERROR("matrix data is nullptr.");
		}
		if (_row >= m_row || _col >= m_col)
		{
			THROW_OUT_OF_RANGE_ERROR("access index out of range");
		}
		return (this->dataPtr + _row * m_col)[ _col]; // column first
	}

	// opencv-style access Mat::at(i,j) read-only version
	inline
	const_ref at(const size_t _row, const size_t _col) const
	{
		if (dataPtr == nullptr)
		{
			THROW_RUNTIME_ERROR("matrix data is nullptr.");
		}
		if (_row >= m_row || _col >= m_col)
		{
			THROW_OUT_OF_RANGE_ERROR("access index out of range");
		}
		return (this->constDataPtr + _row * m_col)[_col]; // column first
	}

	// matlab-style access operator() write version
	inline
	ref operator()(const size_t _row, const size_t _col)
	{
		if (dataPtr == nullptr)
		{
			THROW_RUNTIME_ERROR("matrix data is nullptr.");
		}
		if (_row >= m_row || _col >= m_col)
		{
			THROW_OUT_OF_RANGE_ERROR("access index out of range");
		}
		return (this->dataPtr + _row * m_col)[_col]; // column first
	}

	// matlab-style access operator() read-only version
	inline
	const_ref operator()(const size_t _row, const size_t _col) const
	{
		if (dataPtr == nullptr)
		{
			THROW_RUNTIME_ERROR("matrix data is nullptr.");
		}
		if (_row >= m_row || _col >= m_col)
		{
			THROW_OUT_OF_RANGE_ERROR("access index out of range");
		}
		return (this->constDataPtr + _row * m_col)[_col]; // column first
	}

	// allocate the memory for matrix 
	// no params
	inline
	void create()
	{
		if (m_size > 0)
		{
			m_data = m_allocator.allocate(m_size);
		}
	}

	// allocate the memory for matrix 
	// reload with matrix size
	inline
	void create(const size_t _row, const size_t _col)
	{
		this->release();
		m_row = _row;
		m_col = _col;
		m_size = _row * _col;
		if (m_size > 0)
		{
			m_data = m_allocator.allocate(m_size);
		}
	}

	// allocate the memory for matrix 
	// reload with square matrix size
	inline
	void create(const size_t _squareSize)
	{
		this->release();
		m_row = _squareSize;
		m_col = _squareSize;
		m_size = _squareSize * _squareSize;
		if (m_size > 0)
		{
			m_data = m_allocator.allocate(m_size);
		}
	}

	// free the memory of matrix
	inline
	void release() noexcept
	{
		if (m_data != nullptr)
		{
			m_allocator.deallocate(m_data, m_size);
			m_data = nullptr;
		}
		m_col = m_row = m_size = 0;
	}

	static Mat<T> zeros(const size_t _squareSize);
	static Mat<T> zeros(const size_t _row, const size_t _col);

	static Mat<T> ones(const size_t _squareSize);
	static Mat<T> ones(const size_t _row, const size_t _col);

	static Mat<T> eye(const size_t _squareSize);

	// const data pointer
	inline
	const_pointer constDataPtr() const noexcept
	{ 
		return m_data; 
	}

	// data pointer
	inline
	pointer dataPtr() noexcept
	{ 
		return m_data; 
	}

	// data shape row
	inline
	size_t row() const noexcept
	{ 
		return m_row; 
	}

	// data shape col
	inline
	size_t col() const noexcept
	{ 
		return m_col; 
	}

	// data size
	inline
	size_t size() const noexcept
	{
		return m_size;
	}

	// data Allocator
	inline
	const Allocator& allocator() const noexcept
	{
		return m_allocator;
	}

	// matrix is square
	inline
	bool isSquare() const noexcept
	{
		return m_col == m_row;
	}

	// matrix is empty
	inline
	bool isEmpty() const noexcept
	{
		return m_size == 0;
	}

	

private:
	// Allocator
	Allocator  m_allocator{};
	// data pointer
	pointer m_data{ nullptr };
	// Attributes
	size_t m_col{ 0 };
	size_t m_row{ 0 };
	size_t m_size{ 0 }; // size = row * col

	}; // class Mat

} // namespace ht