#pragma once
/*
  Swappable Engine Factory, I think a better variation to the 
  Factory Pattern
*/
#include <utility>
namespace car_factory {

	// interface to swapable engines
	enum class engine_tag : char { old = 'O', next = 'N' };

	namespace engine_workshop { // hidden from clients
	// interface to swapable engines
		struct IEngine { virtual  engine_tag start() = 0; };
		// switchable engines
		struct old : public IEngine { virtual engine_tag start() override { return engine_tag::old; } };
		struct next : public IEngine { virtual engine_tag start() override { return engine_tag::next; } };
	} // inner

	// facade of the solution
	// this is not ABC
	class Automobile final {

		using IEngine = typename engine_workshop::IEngine ;
		// we use the simple native pointer
		// thus swap idiom is very easy to implement
		mutable IEngine * engine_{};
		// this ctor is visible to the factory method
		// we do not want this as a "converting constructor"
		// ditto we declare it as explicit
		explicit Automobile( IEngine * use_) : engine_(use_) {}
		// no default ctor
		// no nullptr engine
		Automobile() = delete;
		// the factory function declaration
		friend  Automobile assembly_line(engine_tag);
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
inline Automobile assembly_line (engine_tag which_)
{
	// I assume I do not need std::move in here
	// I could also use std::enable_if and std::is_move_constructible
	// in return value type to check the Automobile type movability
	// or I could just leave it to the CL 

	using namespace engine_workshop;

	if (engine_tag::next == which_ )
		return Automobile(new next{});
	if (engine_tag::old == which_)
		return Automobile(new old{});
		
	throw std::runtime_error{ __FUNCSIG__ " Unknown engine_tag ordered?"};
}

}
/*
Copyright 2018 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/