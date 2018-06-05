#pragma once
#include <utility>
namespace dbj_samples {

	// interface to swapable engines
	enum class engine_tag : char { old = 'O', next = 'N' };

	namespace inner { // hidden from clients
	// interface to swapable engines
		struct IEngine { virtual  engine_tag start() = 0; };
		// switchable engines
		struct old : public IEngine { virtual engine_tag start() override { return engine_tag::old; } };
		struct next : public IEngine { virtual engine_tag start() override { return engine_tag::next; } };
	} // inner

	// facade of the solution
	// this is not ABC
	class Automobile final {
		// we use the simple native pointer
		// thus swap idiom is very easy to implement
		mutable inner::IEngine * engine_{};
		// this ctor is visible to the factory method
		// we do not want this as a "converting constructor"
		// ditto we declare it as explicit
		explicit Automobile(inner::IEngine * use_) : engine_(use_) {}
		// no default ctor
		// no nullptr engine
		Automobile() = delete;
		// the factory function declaration
		friend  Automobile automobile_factory(engine_tag);
	public:
		// delegating to the engine
		// inheritance is evil
		engine_tag start() const { return engine_->start(); }
		// it is easy to manage the pointer lifetime
		~Automobile() { delete engine_; }
		/*
		required move semantics 
		NOTE: we can not allow for move or copy assignment
		since this will effectively and quietly
		change the engine "mid flight" 
		*/
		// no copy
		Automobile(const Automobile&) = delete;
		Automobile& operator=(const Automobile&) = delete;
		// move
		Automobile(Automobile&& other_) noexcept { std::swap(this->engine_, other_.engine_); }
		Automobile& operator=(Automobile&& other_) = delete;  // noexcept { std::swap(this->engine_, other_.engine_); return *this; }

	};

	// factory inserts the engine on production line
	// we do not return ABC so we enjoy the value semantics
	inline Automobile automobile_factory(engine_tag which_)
	{
		// I assume I do not need std::move in here
		// I could also use std::enable_if and std::is_move_constructible
		// in return value type to check the Automobile type movability
		// or I could just leave it to the CL 

		if (engine_tag::next == which_ )
			return Automobile(new inner::next{});
		if (engine_tag::old == which_)
			return Automobile(new inner::old{});
		
		throw std::runtime_error{ __FUNCSIG__ " Unknown engine_tag ordered?"};
	}

}