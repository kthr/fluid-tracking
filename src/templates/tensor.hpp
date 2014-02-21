/*
 * tensor.hpp
 *
 *  Created on: Oct 25, 2013
 *      Author: kthierbach
 */

#ifndef TENSOR_HPP_
#define TENSOR_HPP_

#include <memory>
#include <string>
#include <vector>

namespace elib{

template <typename T>
class Tensor
{
	public:
		Tensor()
		{

		}
		Tensor(const Tensor &other)
		: dimensions(other.dimensions), flattened_length(other.flattened_length), rank(other.rank)
		{
			data = std::unique_ptr<T[]>(new T[flattened_length]);
			std::copy(other.data.get(), other.data.get()+flattened_length, data.get());
		}
		Tensor(Tensor &&other) : data(std::move(other.data)), dimensions(std::move(other.dimensions)), flattened_length(other.flattened_length), rank(other.rank)
		{
		}
		Tensor(int rank, int *dimensions) : rank(rank)
		{
			if(rank > 0)
			{
				flattened_length = dimensions[0];
				for(int i=1; i<rank; ++i)
				{
					flattened_length *= dimensions[i];
				}
				this->data = std::unique_ptr<T[]>(new T[flattened_length]);
				this->dimensions =std::vector<int>(rank);
				std::copy(dimensions, dimensions + rank, this->dimensions);
				std::fill_n(this->data.get(), flattened_length, 0);
			}
		}
		Tensor(int rank, int *dimensions, T *data) : Tensor(rank, dimensions)
		{
			std::copy(data, data + this->flattened_length, this->data.get());
		}
		virtual ~Tensor()
		{
		}

		Tensor& operator=(const Tensor &other)
		{
			swap(*this,other);
			return *this;
		}
		Tensor& operator=(Tensor &&other)
		{
			data = std::move(other.data);
			dimensions = std::move(other.dimensions);
			flattened_length = other.flattened_length;
			rank = other.rank;
			return *this;
		}

		const std::vector<int> getDimensions() const
		{
			return dimensions;
		}
		int getFlattenedLength() const
		{
			return flattened_length;
		}
		int getRank() const
		{
			return rank;
		}
		T* getData()
		{
			return data.get();
		}

	private:
		std::unique_ptr<T[]> data = nullptr;
		std::vector<int> dimensions;
		int flattened_length = 0,
			rank = 0;
};

} /* end namespace elib */

#endif /* TENSOR_HPP_ */
