#ifndef ColorC
#define ColorC
#pragma once

/**
 * Number of colors.
 */
static const int 
	nColors = 9;
	
/**
 * Color values.
 */
static const char colors[11] = {
	'p', 'w', 'b', 'y', 'o', 'r',
	'g', 'n', 'e', '*', '0'
};

/**
 * Color symbolic names.
 */
enum color {
	purple	= 'p',
	white	= 'w',
	blue	= 'b',
	yellow	= 'y',
	orange	= 'o',
	red		= 'r',
	green	= 'g',
	black	= 'n',
	engine	= 'e',
	wildcard= '*',
	null	= '0',
};
#endif