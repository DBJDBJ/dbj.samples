#pragma once

/// <summary>
/// inherit as private base this class
/// to make the offspring(s) 
/// no copy
/// no move
/// dbj.org created 
/// </summary>
class no_copy_no_move
{
	no_copy_no_move()  = default ;
	~no_copy_no_move() = default ;
	// no copy
	no_copy_no_move(const no_copy_no_move&) = delete;
	no_copy_no_move& operator=(const no_copy_no_move&) = delete;
	// no move
	no_copy_no_move(no_copy_no_move&&) = delete;
	no_copy_no_move& operator=(no_copy_no_move&&) = delete;
};