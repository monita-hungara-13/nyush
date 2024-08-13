#pragma once

#include "nyush_assert.h"
#include "nyush_types.h"

namespace nyush
{

constexpr size length(const char* in_string)
{
	const char* p = in_string;
	for (; *p;) ++p;
	return p - in_string;
}

constexpr size copy(char* out_dest, const char* in_src, const size in_max_size)
{
	size remaining = in_max_size;
	while (*in_src != '\0' && remaining > 1)
	{
		*out_dest++ = *in_src++;
		--remaining;
	}
	*out_dest = '\0';

	return in_max_size - remaining;
}

constexpr int64 compare(const char* in_left, const char* in_right)
{
	while ((*in_left != '\0' && *in_right != '\0') && *in_left == *in_right)
	{
		++in_left;
		++in_right;
	}

	return (*in_left == *in_right) ? 0 : (*in_left > *in_right) ? 1 : -1;
}

constexpr bool equals(const char* in_left, const char* in_right)
{
	while ((*in_left != '\0' && *in_right != '\0') && *in_left == *in_right)
	{
		++in_left;
		++in_right;
	}

	return *in_left == *in_right;
}

inline char* tokenize(
	char* in_buffer, const char* in_delimiters, char*& inout_next)
{
	char* current;
	char* previous;

	if (in_buffer)
	{
		current = in_buffer;
		previous = in_buffer;
	}
	else [[likely]]
	{
		current = inout_next;
		previous = inout_next;
	}

	size delimiters_count = length(in_delimiters);
	auto current_is_delimiter = [&]()
	{
		for (size i = 0; i < delimiters_count; ++i)
		{
			if (*current == in_delimiters[i])
			{
				return true;
			}
		}

		return false;
	};

	inout_next = nullptr;
	while (current && (*current != '\0'))
	{
		if (current_is_delimiter())
		{
			do
			{
				*current++ = '\0';
			} while (*current != '\0' && current_is_delimiter());

			if (*current != '\0') [[likely]]
			{
				inout_next = current;
			}
			else
			{
				inout_next = nullptr;
			}

			break;
		}

		++current;
	}

	return previous;
}

inline void join(const char** in_strings, size in_count, char* out_buffer,
	const size in_max_size, char in_separator)
{
	size remaining = in_max_size;
	char* buffer = out_buffer;
	for (size i = 0; i < in_count; ++i)
	{
		size written = copy(buffer, in_strings[i], remaining);

		remaining = remaining - written;
		if (remaining <= 1)
		{
			return;
		}

		if (i - 1 != in_count)
		{
			buffer = buffer + written;
			*buffer++ = in_separator;
		}
	}
}

struct charbuff
{
	explicit consteval charbuff(const char* in_string)
		: _buffer(in_string), _length(nyush::length(in_string)){};
	consteval const size length() const { return _length; };
	consteval operator const char*() const { return _buffer; };

private:
	const char* _buffer;
	const size _length;
};

template <size Size>
struct buffer
{
	explicit consteval buffer();

	constexpr const size length() const;
	constexpr operator const char*() const;
	char& operator[](size in_index);
	void append(const char* in_string);
	int32 find(const char* in_substring);
	void clear();

private:
	char _buffer[Size];
	size _length;
};

} // namespace nyush

template <nyush::size Size>
consteval nyush::buffer<Size>::buffer() : _length(0)
{
	for (size i = 0; i < Size; ++i)
	{
		_buffer[i] = '\0';
	}
}

template <nyush::size Size>
constexpr const nyush::size nyush::buffer<Size>::length() const
{
	return _length;
}

template <nyush::size Size>
constexpr nyush::buffer<Size>::operator const char*() const
{
	return _buffer;
}

template <nyush::size Size>
char& nyush::buffer<Size>::operator[](size in_index)
{
	nyush_assert(in_index <= _length && "out of bounds");

	return _buffer[in_index];
}

template <nyush::size Size>
void nyush::buffer<Size>::append(const char* in_value)
{
	const size length = nyush::length(in_value);
	nyush_assert(length < Size && "not enough space");

	while ((_length < Size - 1) && (*in_value != '\0'))
	{
		_buffer[_length++] = *in_value++;
	}
	_buffer[_length] = '\0';
}

template <nyush::size Size>
nyush::int32 nyush::buffer<Size>::find(const char* in_substring)
{
	const size replace_length = nyush::length(in_substring);

	// NOTE: Catch this here to avoid underflow when subtracting two unsigned
	// values, also early exit.
	if (replace_length > _length)
	{
		return -1;
	}

	const size max_index = _length - replace_length;
	for (size buffer_index = 0; buffer_index <= max_index; ++buffer_index)
	{
		size substring_index = 0;
		while (substring_index < replace_length &&
			   _buffer[buffer_index + substring_index] ==
				   in_substring[substring_index])
		{
			++substring_index;
		}
		if (substring_index == replace_length)
		{
			return buffer_index;
		}
	}

	return -1;
}

template <nyush::size Size>
void nyush::buffer<Size>::clear()
{
	*_buffer = '\0';
	_length = 0;
}
