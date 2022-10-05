//
// Created by luigi on 21/09/22.
//

#ifndef EMPI_PROJECT_TAG_HPP
#define EMPI_PROJECT_TAG_HPP

namespace empi {
struct Tag {
 public:
  explicit constexpr Tag(int value) : value(value) {}
  constexpr bool operator==(Tag tag) const {
	return this->value == tag.value;
  }
  constexpr bool operator!=(int val) const {
	return this->value != val;
  }
  constexpr bool operator>=(int val) const {
	return this->value >= val;
  }
  constexpr bool operator<=(int val) const {
	return this->value <= val;
  }
  constexpr bool operator>(int val) const {
	return this->value > val;
  }
  constexpr bool operator<(int val) const {
	return this->value < val;
  }

  const int value;
};

static constexpr Tag NOTAG{-2};
}

#endif //EMPI_PROJECT_TAG_HPP
