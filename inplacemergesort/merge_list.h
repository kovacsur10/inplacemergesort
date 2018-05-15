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

	void initialize(const std::vector<int>& l1, const std::vector<int>& l2);

	void block_merge_forward();

	void block_merge_forward_worker(size_t& offset, size_t& first_part_length);

	void prepare();

	void main();

	std::ptrdiff_t main_worker(const std::ptrdiff_t offset);

	bool is_perfect_square(const size_t num) const;

	void swap(const size_t i, const size_t j);

	void swap(const position& pos1, const position& pos2);

	void swap(std::vector<int>::iterator& i, std::vector<int>::iterator& j);

	void sort(const position& buffer, const position& part1, const position& part2);

	size_t length(const position& pos);
};

#endif
