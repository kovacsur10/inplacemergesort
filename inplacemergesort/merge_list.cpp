#include "merge_list.h"

#include <cmath>
#include <algorithm> // upper_bound, reverse, distance

// Construct the first list, from 2 initializer lists.
merge_list::merge_list(const std::initializer_list<int>& l1, const std::initializer_list<int>& l2) : merged(false){
	initialize(l1, l2);
}

// Construct the first list, from 2 vectors.
merge_list::merge_list(const std::vector<int>& l1, const std::vector<int>& l2) : merged(false){
	initialize(l1, l2);
}

void merge_list::initialize(const std::vector<int>& l1, const std::vector<int>& l2){
	bool second_is_copied = false;

	// If one of the sublists is shorter, than the sqrt(length), it must go to the beginning of the vector.
	double limit = sqrt(l1.size() + l2.size());
	block_size = static_cast<unsigned>(limit);
	if(l2.size() < limit){ // Note: This is the only case when the second sublist goes to the beginning.
						   // Add the second sublist to the list.
		for(auto e : l2){
			data.push_back(e);
		}
		init_lengths.first = l2.size();
		init_lengths.second = l1.size();
		second_is_copied = true;
	}

	// Add the first sublist to the list.
	for(auto e : l1){
		data.push_back(e);
	}

	// If the second sublist is in the front of the list, do not add it again.
	if(!second_is_copied){
		// Add the second sublist to the list.
		for(auto e : l2){
			data.push_back(e);
		}
		init_lengths.first = l1.size();
		init_lengths.second = l2.size();
	}

	//This part is only for the verification!
	for(const auto e : l1){
		verification_data.push_back(e);
	}
	for(const auto e : l2){
		verification_data.push_back(e);
	}
	std::sort(verification_data.begin(), verification_data.end());
}

void merge_list::block_merge_forward(){
	double limit = sqrt(data.size());
	if(init_lengths.first > limit){
		return;
	}

	size_t offset = 0;
	size_t first_length = init_lengths.first;
	for(unsigned i = 0; i < init_lengths.first; i++){
		block_merge_forward_worker(offset, first_length);
	}

	merged = true;
}

void merge_list::block_merge_forward_worker(size_t& offset, size_t& first_part_length){
	auto upper = std::upper_bound(data.begin() + offset, data.end(), data[offset]);
	std::reverse(data.begin() + offset, data.begin() + offset + first_part_length);
	std::reverse(data.begin() + offset + first_part_length, upper);
	std::reverse(data.begin() + offset, upper);
	first_part_length--; 
	offset = upper - data.begin() - first_part_length;
}

void merge_list::merge(){
	if(!merged){
		block_merge_forward();
		prepare();
		main();
	}
}

void merge_list::prepare(){
	if(!merged){
		if(is_perfect_square(data.size())){
			// Move the greatest elements to the front.
			unsigned first = 1;
			unsigned second = 1;
			for(unsigned i = 0; i < block_size; i++){
				if(data[init_lengths.first - first] > data[data.size() - second]){
					first++;
				} else{
					second++;
				}
			}
			first--;
			second--;
			std::reverse(data.begin() + init_lengths.first, data.end()); // Last elements goes to front
			std::reverse(data.begin() + init_lengths.first + second, data.end()); // Second part - last elements moves back.
			std::reverse(data.begin(), data.begin() + init_lengths.first + second); // The buffer comes to the front.
			std::reverse(data.begin() + block_size, data.begin() + init_lengths.first + second); // The first part - buffer moves back.

			buffer = data.begin();
			end = data.end();
			end_of_normal = end;
		} else{ // Preparation is needed according to the paper.
			// Create the named ranges - A B C D.
			// A positions first element is the first character,
			//             the second element is the last + 1.
			position a, b, c, d, f;
			b.second = data.size();
			a.second = init_lengths.first;
			c.first = init_lengths.first - block_size;
			d.first = init_lengths.second % block_size;
			if(d.first == 0){
				d.first = data.size() - block_size;
			} else{
				d.first = data.size() - d.first;
			}
			f.first = 0;
			f.second = init_lengths.first % block_size;
			if(f.second == 0){
				f.second = block_size;
			}

			// 0. Mark the greatest elements.
			a.first = a.second;
			b.first = b.second;
			for(unsigned i = 0; i < block_size; i++){
				if(data[a.first-1] >= data[b.first-1]){
					a.first--;
				} else{
					b.first--;
				}
			}
			c.second = a.first;
			d.second = b.first;

			// 1. Swap the C and B.
			for(size_t i = c.first; i < c.second; i++){
				swap(i, b.first - c.first + i);
			}
			position tmp = c;
			c = b;
			b = tmp;

			// 2. Merge the tail (DC) using the buffer (BA).
			position e = std::make_pair(d.first, c.second);
			if(c.first < d.first){
				e.first = c.first;
			}
			position buf = std::make_pair(a.second-length(e), a.second);
			position h = std::make_pair(b.first, a.second);
			sort(buf, d, c);
			for(size_t i = a.second - length(e); i < a.second; i++){
				swap(i, e.first + i - a.second + length(e));
			}

			if(f.second - f.first < block_size){
				// 3. Create G from the second part's first block.
				position g = std::make_pair(init_lengths.first, init_lengths.first + block_size);

				// 4. Merge F and G using the rightmost len(F) elements of the buffer (BA).
				buf = std::make_pair(a.second - length(f), a.second + block_size); // t1 elements form buffer + G
				sort(buf, f, g);

				// 5. Swap H and F.
				buf = std::make_pair(a.second - length(f), a.second);
				swap(buf, f);

				// 6. Swap the buffer with the first s-sized block.
				position j = std::make_pair(f.second, f.second + block_size);
				swap(h, j);
				buffer = data.begin() + length(f);
			} else{
				// 3. Swap the buffer with the first block
				swap(h, f);
				buffer = data.begin();
			}
			end = data.end();
			end_of_normal = data.end() - (e.second - e.first);
		}
	}
}

// Note: The buffer cannot be longer, than the len(part1)+len(part2).
void merge_list::sort(const position& buffer, const position& part1, const position& part2){
	unsigned offset1 = 0;
	unsigned offset2 = 0;
	for(size_t i = buffer.first; i < buffer.second; i++){
		if(offset2 == length(part2) || (offset1 < length(part1) && data[part1.first + offset1] <= data[part2.first + offset2])){
			swap(part1.first + offset1, i);
			offset1++;
		} else{
			swap(part2.first + offset2, i);
			offset2++;
		}
	}
}

void merge_list::main(){
	if(!merged){
		// Move the blocks into order based on the tail elements.
		std::vector<std::pair<unsigned, unsigned>> blocks;
		unsigned block_count = (data.end() - buffer) / block_size;
		unsigned initial_offset = buffer - data.begin();
		for(unsigned i = 1; i < block_count; i++){
			blocks.emplace_back(i, data[block_size - 1 + i * block_size + initial_offset]);
		}
		std::sort(blocks.begin(), blocks.end(), [](std::pair<unsigned, unsigned> i, std::pair<unsigned, unsigned> j){ return i.second < j.second; });
		for(unsigned i = 0; i < blocks.size(); i++){
			for(unsigned j = 0; j < block_size; j++){
				if(i+1 != blocks[i].first){
					swap((i + 1) * block_size + j + initial_offset, blocks[i].first * block_size + j + initial_offset);
				}
			}
			// Update the remaining movements.
			for(unsigned j = i + 1; j < blocks.size(); j++){
				if(blocks[j].first == i+1){
					blocks[j].first = blocks[i].first;
				}
			}
		}

		// Run the algorithm in loop.
		std::ptrdiff_t offset = main_worker(0);
		while(offset > 0){
			offset = main_worker(offset);
		}

		// Do the cleanup phase.
		// Move the buffer to the end.
		std::vector<int>::iterator less;
		for(std::vector<int>::iterator it = buffer + block_size; it != end; it++){
			less = it - block_size;
			swap(it, less);
		}
		// Sort the buffer.
		std::sort(end - block_size, end);

		merged = true;
	}
}

std::ptrdiff_t merge_list::main_worker(const std::ptrdiff_t offset){
	// Set the leftmost element of the remainings.
	std::vector<int>::iterator first = buffer + block_size;

	// Find the second series.
	std::vector<int>::iterator second = first + (offset == 0 ? block_size : offset);
	while(second < end && (*(second - 1) <= *(second))){
		if(second+block_size <= end_of_normal){
			second += block_size;
		}else{
			second = end_of_normal;
		}
	}
	// Only one series can be created. The loop can be halted. Cleanup phase can begin.
	if(!(first <= second && second < end)){
		return -1;
	}

	// We have the two series. Merge them.
	std::vector<int>::iterator last = end - second >= block_size ? second + block_size : end;
	std::vector<int>::iterator quit_position = second;
	std::vector<int>::iterator beginning = buffer;
	while(first < quit_position){
		if(second == end || (first != end && *first <= *second)){
			swap(beginning, first);
			beginning++;
			first++;
		} else{
			swap(beginning, second);
			beginning++;
			second++;
		}
	}
	buffer = second - block_size;
	return std::distance(second, last);
}

void merge_list::swap(const position& pos1, const position& pos2){
	position p1 = pos1;
	position p2 = pos2;
	if(p1.first > p2.first){
		p1 = pos2;
		p2 = pos1;
	}

	for(size_t i = p1.second - length(p2); i < p1.second; i++){
		swap(i, p2.first + i - p1.second + length(p2));
	}
}

void merge_list::swap(const size_t i, const size_t j){
	unsigned tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}

void merge_list::swap(std::vector<int>::iterator& i, std::vector<int>::iterator& j){
	unsigned tmp = *i;
	(*i) = *j;
	*j = tmp;
}

bool merge_list::is_perfect_square(const size_t num) const{
	unsigned i = 0;
	while(i * i < num){
		i++;
	}
	return i * i == num;
}

std::ostream& operator<<(std::ostream& os, const merge_list& list){
	os << "[ ";
	for(unsigned i = 0; i < list.data.size(); i++){
		os << list.data[i];
		if(i < list.data.size() - 1){
			os << ", ";
		}
	}
	os << " ]";
	return os;
}

size_t merge_list::length(const position& pos){
	return pos.second - pos.first;
}

bool merge_list::verify() const{
	if(data.size() != verification_data.size())
		return false;
	for(size_t i = 0; i < data.size(); i++){
		if(data[i] != verification_data[i]){
			return false;
		}
	}
	return true;
}
