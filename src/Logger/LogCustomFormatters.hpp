/**
 * @file LogCustomFormatters.hpp
 * @author SW
 * @version 0.2.1
 * @date 2024-05-10
 *
 * @copyright Copyright (c) 2024 SW
 */
#pragma once

#ifdef SW_LOGGER_USE_FMT
	#define formatns fmt

	#include <spdlog/fmt/fmt.h>
#else
	#define formatns std

	#include <format>
#endif

/**
 * @brief This macro is used to define a new formatter for a custom type.
 *
 * @param T The type for which the formatter is defined.
 *
 * The formatter is defined in the formatns namespace. Either fmt or std. Based on the USE_FMT_FOR_LOGGING macro.
 *
 * Must be used in the global namespace. And closed with the END_CAST_FORMATTER macro.
 *
 * Example usage:
 *
 *     BEGIN_CAST_FORMATTER(std::filesystem::path);
 *     FORMATTER_CAST(std::string, value.string());
 *     END_CAST_FORMATTER;
 *
 *     The above code will define a formatter for std::filesystem::path type.
 *
 *     Available variables:
 *       - value: The value of the type to be formatted.
 *       - ctx: The format context.
 */
#define BEGIN_CAST_FORMATTER(T)                                                                            \
	namespace formatns                                                                                     \
	{                                                                                                      \
		template <>                                                                                        \
		struct formatter<T> : formatter<std::string>                                                       \
		{                                                                                                  \
			formatns::format_context::iterator format(const T& value, formatns::format_context& ctx) const \
			{

#define END_CAST_FORMATTER \
	}                      \
	}                      \
	;                      \
	}

/**
 * @brief This macro is used to format a value using the custom formatter and add it to the output buffer.
 *
 * @param T The type of the value to be formatted.
 * @param value The value to be formatted.
 *
 * Example usage:
 *      FORMATTER_CAST(std::string, value.string());
 */
#define FORMATTER_CAST(T, value) return formatns::formatter<T>().format(value, ctx)

/**
 * @brief This macro is used to define a new formatter for a custom type.
 *
 * @param T The type for which the formatter is defined.
 *
 * The formatter is defined in the formatns namespace. Either fmt or std. Based on the USE_FMT_FOR_LOGGING macro.
 *
 * Must be used in the global namespace. And closed with the END_ADV_FORMATTER macro.
 *
 * Example usage:
 *
 *     struct Vec3
 *     {
 *         float x, y, z;
 *     };
 *
 *     BEGIN_ADV_FORMATTER(Vec3);
 *
 *     *it++ = '[';
 *     it    = FORMATTER_FORMAT(float, value.x);
 *     *it++ = ',';
 *     it    = FORMATTER_FORMAT(float, value.y);
 *     *it++ = ',';
 *     it    = FORMATTER_FORMAT(float, value.z);
 *     *it++ = ']';
 *
 *     END_ADV_FORMATTER;
 *
 *     The above code will define a formatter for std::filesystem::path type.
 *
 *     Available variables:
 *       - value: The value of the type to be formatted.
 *       - it: The iterator to the output buffer.
 *       - ctx: The format context.
 */
#define BEGIN_ADV_FORMATTER(T)                                                                            \
	namespace formatns                                                                                    \
	{                                                                                                     \
		template <>                                                                                       \
		struct formatter<T, char>                                                                         \
		{                                                                                                 \
			constexpr formatns::format_parse_context::iterator parse(formatns::format_parse_context& ctx) \
			{                                                                                             \
				formatns::format_parse_context::iterator it = ctx.begin(), end = ctx.end();               \
				if (it != end && *it != '}')                                                              \
					throw formatns::format_error("invalid format");                                       \
				return it;                                                                                \
			}                                                                                             \
			formatns::format_context::iterator format(T value, formatns::format_context& ctx) const       \
			{                                                                                             \
				formatns::format_context::iterator it = ctx.out();

#define END_ADV_FORMATTER \
	return it;            \
	}                     \
	}                     \
	;                     \
	}

/**
 * @brief This macro is used to format a value using the custom formatter and add it to the output buffer.
 *
 * @param T The type of the value to be formatted.
 * @param value The value to be formatted.
 *
 * Example usage:
 *      it    = FORMATTER_FORMAT(float, value);
 */
#define FORMATTER_FORMAT(T, value) formatns::formatter<T, char>().format(value, ctx)
