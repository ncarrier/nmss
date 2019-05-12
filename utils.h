#ifndef UTILS_H_
#define UTILS_H_

/* GCC is awesome. */
#define ARRAY_SIZE(arr) \
	(sizeof(arr) / sizeof((arr)[0]) \
		+ sizeof(typeof(int[1 - 2 * \
		!!__builtin_types_compatible_p(typeof(arr), \
		typeof(&arr[0]))])) * 0)

#endif /* UTILS_H_ */
