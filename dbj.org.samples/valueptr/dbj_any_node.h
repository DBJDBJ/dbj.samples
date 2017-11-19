#pragma once
/*
I looked into this: https://github.com/LoopPerfect/valuable
and I decided it is not necessary. At all. Hence my AnyNode to prove it.

2017 NOV 18

*/

namespace dbj {
	
	template<typename T>
	inline std::string address_as_string(T * p) {
		char buf[9] = { 0x0 };
		sprintf_s(buf, 9,"%p", static_cast<void *>(p));
		return std::string(buf);
	}

	struct AnyNode {

		typedef typename std::any handle_t;

		std::string		uid{};
		std::string		name{};
		handle_t		left{};
		handle_t		right{};

		static AnyNode val(const handle_t & tp_) {
			try {
				return std::any_cast<AnyNode>( tp_ );
			}
			catch (std::bad_any_cast & x) {
				dbj::trace("Exception at %s(%d) [%s]", __FILE__, __LINE__, x.what());
				throw dbj::Exception(x.what());
			}
		}

		static  const bool empty(const handle_t & tp_) {
			return ! (tp_).has_value();
		}

		/*
		AnyNode(AnyNode && other) {  swap(*this, other); }
		AnyNode(const AnyNode  & other) { swap(const_cast<AnyNode&>(*this), const_cast<AnyNode&>(other)); }

		friend void swap(AnyNode & lval, AnyNode & rval)
		{
		lval.name = rval.name;
		lval.left = rval.left;   //rval.left = nullopt;
		lval.right = rval.right; //rval.right = nullopt;
		}

		~AnyNode() = default; // trivially destructible
		// ~AnyNode() noexcept {}; // no throw destructible
		// ~AnyNode() {}; // destructible
		AnyNode() {};
		*/
		AnyNode(
			const std::string   & name,
			const handle_t		& left_ = handle_t{},
			const handle_t		& right_ = handle_t{}
		)
			: name{ name },
			left{ left_ },
			right{ right_ },
			uid{ address_as_string(this) }
		{
		}
#if 0
		void print() {
			dbj::print("('", this->name, "'");
			if (! empty(left)) {
				dbj::print("\n\t(left : ");
				val(left).print();
				dbj::print(")");
			}
			if (! empty(right)) {
				dbj::print("\n\t(right: ");
				val(right).print();
				dbj::print(")");
			}
			dbj::print("\n)");
		}
#endif
	};

	/*
	 AnyNode visitor signature
	*/
	using AnyVisitor = bool(AnyNode &);
	// typedef typename std::function<AnyVisitor> AnyNodeVisitor;

	/*
	3 kinds of binary tree traversal
	*/

	inline void preorder( AnyNode & root_, AnyVisitor f) {
		if (!f(root_)) return; // process, stop if false return
		if (!AnyNode::empty(root_.left ))  preorder(AnyNode::val(root_.left), f); // left subtree
		if (!AnyNode::empty(root_.right))  preorder(AnyNode::val(root_.right), f); // right subtree
	}

	inline void inorder( AnyNode & root_, AnyVisitor f) {
		if (!AnyNode::empty(root_.left))  inorder(AnyNode::val(root_.left), f); // left subtree
		if (!f(root_)) return; // process, stop if false return
		if (!AnyNode::empty(root_.right))  inorder(AnyNode::val(root_.right), f); // right subtree
	}

	inline void postorder( AnyNode & root_, AnyVisitor f) {
		if (!AnyNode::empty(root_.left))  postorder(AnyNode::val(root_.left), f); // left subtree
		if (!AnyNode::empty(root_.right))  postorder(AnyNode::val(root_.right), f); // right subtree
		if (!f(root_)) return; // process, stop if false return
	}

} // dbj

#ifdef DBJ_TESTING_EXISTS
#pragma region dbj testing
namespace tree_testing {

	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": value ptr ")) {

		using dbj::AnyNode;

		AnyNode root = AnyNode("root", AnyNode( "L" ), AnyNode( "R" )	);

			// root.left = root; // copy of root assigned to left
			// root.right = root; // and we don't have cyclic references

			// root.print();

			 auto visiteur = [] (AnyNode & n) -> bool {
				 dbj::print("(name: '", n.name, "', uid:", n.uid, ")");
				 return true;
			 };

			 dbj::print("\n");
			 dbj::preorder(root, visiteur);
			 dbj::print("\n");
	}
}
#pragma endregion
#endif