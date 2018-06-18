#pragma once
namespace dbj_samples {
	/// <summary>
	/// inherit as private base this class
	/// to make the offspring(s) 
	/// no copy
	/// no move
	/// dbj.org created 
	/// </summary>
	struct no_copy_no_move
	{
	protected:
		// no copy
		no_copy_no_move(const no_copy_no_move&) = delete;
		no_copy_no_move& operator=(const no_copy_no_move&) = delete;
		// no move
		no_copy_no_move(no_copy_no_move&&) = delete;
		no_copy_no_move& operator=(no_copy_no_move&&) = delete;
	public:
		no_copy_no_move() noexcept {}
		virtual ~no_copy_no_move() {}
	};
} // dbj_samples