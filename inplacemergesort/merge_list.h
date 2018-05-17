#ifndef MERGE_LIST_H
#define MERGE_LIST_H 

#include <vector>
#include <utility>
#include <ostream>

class merge_list{
public:
	merge_list(const std::initializer_list<int>& l1, const std::initializer_list<int>& l2);

	merge_list(const std::vector<int>& l1, const std::vector<int>& l2);
	
	inline bool is_merged() const{
		return merged;
	}

	void merge();

	friend std::ostream& operator<<(std::ostream& os, const merge_list& list);

	bool verify() const;

private:
	typedef std::pair<size_t, size_t> position;

	std::vector<int> data;
	std::vector<int>::iterator buffer;
	std::vector<int>::iterator end;
	std::vector<int>::iterator end_of_normal; //beginning of part E
	std::vector<int> verification_data;
	std::pair<size_t, size_t> init_lengths;
	bool merged;
	unsigned block_size;

	// This function is the private constructor (it is shared between the public constructors).
	void initialize(const std::vector<int>& l1, const std::vector<int>& l2);

	// This function does the block merge forward algorithm. (One of the two sublist is way shorter than the other one.)
	void block_merge_forward();

	// This function is the repeatable logic in the block merge forward algorithm.
	void block_merge_forward_worker(size_t& offset, size_t& first_part_length);

	// This function does the preparations if the source list is not a perfect square.
	void prepare();

	// This function runs the algorithm from the paper.
	void main();

	// This function is the repeatable logic in the main algorithm.
	std::ptrdiff_t main_worker(const std::ptrdiff_t offset);

	// Returns if a "num" is a perfect square number (n * n == num) or not.
	bool is_perfect_square(const size_t num) const;

	// This function swaps the "i"th and "j"th elements of the "data" vector.
	void swap(const size_t i, const size_t j);

	// This function swaps the two sublists of the "data" vector indicated by the "pos1" and the "pos2".
	void swap(const position& pos1, const position& pos2);

	// This function swaps the "i"th and "j"th elements of the "data" vector.
	void swap(std::vector<int>::iterator& i, std::vector<int>::iterator& j);

	void sort(const position& buffer, const position& part1, const position& part2);

	// Returns the length of the "pos" vector.
	size_t length(const position& pos);
};

#endif
