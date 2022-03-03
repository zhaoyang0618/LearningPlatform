/// Sort the standard input alphabetically.
/// Read lines of text, sort them, and print the results to the standard output.
/// If the command line names a file, read from that file. Otherwise, read from
/// the standard input. The entire input is stored in memory, so don’t try
/// this with input files that exceed available RAM.
///
/// Comparison uses a locale named on the command line, or the default, unnamed
/// locale if no locale is named on the command line.
/// 原来使用import，但是VS编译器似乎还不支持
#include <cerrno>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <system_error>
#include <vector>
#include <iomanip>
#include <format>

//import <algorithm>;
//import <fstream>;
//import <initializer_list>;
//import <iostream>;
//import <iterator>;
//import <locale>;
//import <string>;
//import <system_error>;
//import <vector>;

template<class C>
struct text : std::basic_string<C>
{
	using super = std::basic_string<C>;
	constexpr text() noexcept : super{} {}
	text(text&&) = default;
	text(text const&) = default;
	text& operator=(text const&) = default;
	text& operator=(text&&) = default;
	constexpr explicit operator bool() const noexcept {
		return not this->empty();
	}
};

/// Read lines of text from @p in to @p iter. Lines are appended to @p iter.
/// @param in the input stream
/// @param iter an output iterator
template<class Ch>
auto read(std::basic_istream<Ch>& in) -> std::vector<text<Ch>>
{
	std::vector<text<Ch>> result;
	text<Ch> line;
	while (std::getline(in, line))
		result.emplace_back(std::move(line));
	return result;
}

/// Main program.
int Test0102(int argc, char* argv[])
try
{
	// Throw an exception if an unrecoverable input error occurs, e.g.,
	// disk failure.
	std::cin.exceptions(std::ios_base::badbit);
	// Part 1. Read the entire input into text. If the command line names a file,
	// read that file. Otherwise, read the standard input.
	std::vector<text<char>> text; ///< Store the lines of text here
	if (argc < 2)
		text = read(std::cin);
	else
	{
		std::ifstream in{ argv[1] };
		if (not in)
		{
			std::cout << argv[1] << ": " << std::system_category().message(errno) << '\n';
			return EXIT_FAILURE;
		}
		text = read(in);
	}
	// Part 2. Sort the text. The second command line argument, if present,
	// names a locale, to control the sort order. Without a command line
	// argument, use the default locale (which is obtained from the OS).
	std::locale const& loc{ std::locale(argc >= 3 ? argv[2] : "") };
	std::collate<char> const& collate{ std::use_facet<std::collate<char>>(loc) };
	std::ranges::sort(text,
		[&collate](auto const& a, auto const& b)
		{
			return collate.compare(to_address(cbegin(a)), to_address(cend(a)),
				to_address(cbegin(b)), to_address(cend(b))) < 0;
		}
	);
	// Part 3. Print the sorted text.
	for (auto const& line : text)
		std::cout << line << '\n';

	return EXIT_SUCCESS;
}
catch (std::exception& ex)
{
	std::cerr << "Caught exception: " << ex.what() << '\n';
	std::cerr << "Terminating program.\n";
	std::exit(EXIT_FAILURE);
}
catch (...)
{
	std::cerr << "Caught unknown exception type.\nTerminating program.\n";
	std::exit(EXIT_FAILURE);
}


/// <summary>
/// 输出格式的控制
/// </summary>
void testFormattedOut()
{
	std::cout << " N N^2 N^3\n";
	for (int i{ 1 }; i != 21; ++i)
		std::cout << std::setw(2) << i
		<< std::setw(6) << i * i
		<< std::setw(7) << i * i * i
		<< '\n';

	std::cout << std::format("x: {}, y: {}, z: {}\n", 10, 20, 30) << std::endl;
	std::cout << std::format("x: {2}, y: {1}, z: {0}\n", 10, 20, 30) << std::endl;
	std::cout << std::format("'{0:c}': {0:#04x} {0:0>#10b} |{0:{1}d}| {2:s}\n", '*', 4, "str") << std::endl;

	//
	int constexpr low{ 1 }; ///< Minimum value for the table
	int constexpr high{ 10 }; ///< Maximum value for the table
	int constexpr colwidth{ 4 }; ///< Fixed width for all columns
	// First print the header.
	std::cout << std::format("{1:>{0}c}|", colwidth, '*');
	for (int i{ low }; i <= high; i = i + 1)
		std::cout << std::format("{1:{0}}", colwidth, i);
	std::cout << '\n';
	// Print the table rule by using the fill character.
	std::cout << std::format("{2:->{0}}+{2:->{1}}\n",
		colwidth, (high - low + 1) * colwidth, "");
	// For each row...
	for (int row{ low }; row <= high; row = row + 1)
	{
		std::cout << std::format("{1:{0}}|", colwidth, row);
		// Print all the columns.
		for (int col{ low }; col <= high; col = col + 1)
			std::cout << std::format("{1:{0}}", colwidth, row * col);
		std::cout << '\n';
	}

}

