#pragma once
/*
I looked into this: https://github.com/LoopPerfect/valuable
and I decided it is not necessary. 
Hence dbj::AnyNode Proof Of Concept (POC) 

2017 NOV 18   dbj.org created

*/

namespace dbj {
	
	template<typename T>
	inline std::string address_as_string(T * p) {
		char buf[9] = { 0x0 };
		sprintf_s(buf, 9,"%p", static_cast<void *>(p));
		return std::string(buf);
	}

	/*
	Desing of this binary tree is:
	extremely simple binary node
	all operations through visitors
	*/
	struct AnyNode final {

		typedef typename std::any handle_t;

		/*
		make visitors more inteligent
		*/
		enum class where_next : int { left = -1, nowhere = 0, right = 1 };
		/*
		encapsulate visitors inteligence
		compate two any nodes and return where visitor should go next
		left or right or nowhere
		this can be used in any kind of operation supported by visitor
		*/
		using DecisionLogic = AnyNode::where_next(const AnyNode &, const AnyNode &);
		/*
		AnyNode visitor signature
		*/
		using AnyVisitor = bool(AnyNode &);
		// typedef typename std::function<AnyVisitor> AnyNodeVisitor;

		// uid is absolutely not changeable
		const			std::string		uid{};

		mutable			handle_t		data{};
		mutable			std::string		name{};
		handle_t		left{};
		handle_t		right{};

		static AnyNode val(const handle_t & tp_) {
			try {
				return std::any_cast<AnyNode>(tp_);
			}
			catch (std::bad_any_cast & x) {
				dbj::trace("Exception at %s(%d) [%s]", __FILE__, __LINE__, x.what());
				throw dbj::Exception(x.what());
			}
		}

		static  const bool empty(const handle_t & tp_) {
			return !(tp_).has_value();
		}

		/*
		the only constructor requires data paramater only
		left_ and right_ are AnyNodes handler's
		*/
		template<typename DT>
		AnyNode(
			const dbj::Any<DT>	& dta_ ,
			const std::string   & name_ = std::string{},
			const handle_t		& left_ = handle_t{},
			const handle_t		& right_ = handle_t{}
		)
			: data{ dta_ },  // store data in std::any
			  name{ name },
			  left{ left_ }, right{ right_ },
			  uid{ address_as_string(this) }
		{
			// if not user defined name is uid 
			if (this->name.empty()) this->name = uid;
		}

		/* 
		   append node to this node into the direction 
		   given by the leader.

		   if direction is nowhere do nothing. this is 
		   potential for silent error which is a bad thing

		   note: remember that false return from here may stop the visitor traversal
		*/
		bool add(where_next leader, const AnyNode & node_to_be_inserted_)
		{
			if (leader == where_next::nowhere) {
				dbj::trace("%s(%d) : insert nowhere?", __FILE__, __LINE__ );
				return true;
			}
			if (leader == where_next::left) {
				this->left = node_to_be_inserted_;
			}
			if (leader == where_next::right) {
				this->right = node_to_be_inserted_;
			}
			return true;
		}
	}; // AnyNode

	 /*	3 kinds of binary tree traversal	*/

	inline void preorder( AnyNode & root_, AnyNode::AnyVisitor f) {
		if (!f(root_)) return; // process, stop if false return
		if (!AnyNode::empty(root_.left ))  preorder(AnyNode::val(root_.left), f); // left subtree
		if (!AnyNode::empty(root_.right))  preorder(AnyNode::val(root_.right), f); // right subtree
	}

	inline void inorder( AnyNode & root_, AnyNode::AnyVisitor f) {
		if (!AnyNode::empty(root_.left))  inorder(AnyNode::val(root_.left), f); // left subtree
		if (!f(root_)) return; // process, stop if false return
		if (!AnyNode::empty(root_.right))  inorder(AnyNode::val(root_.right), f); // right subtree
	}

	inline void postorder( AnyNode & root_, AnyNode::AnyVisitor f) {
		if (!AnyNode::empty(root_.left))  postorder(AnyNode::val(root_.left), f); // left subtree
		if (!AnyNode::empty(root_.right))  postorder(AnyNode::val(root_.right), f); // right subtree
		if (!f(root_)) return; // process, stop if false return
	}

	/*
	 With AnyVisitor and AnyNode traversals we can do anything wee need on the tree made of AnyNodes.

	 AnyVisitor can be lambda, functor, function or anything else 
	 that is callable and confirms to AnyVisitor signature
	*/

	namespace anyvisitors {

		inline AnyNode::where_next no_logic(const AnyNode &, const AnyNode &) {
			return AnyNode::where_next::nowhere;
		}

		class Inserter final {

			AnyNode node_to_be_inserted_{};
			AnyNode::DecisionLogic insertion_logic ;

		public:

			explicit Inserter( const AnyNode & new_node, 
				/*
				we give insertion logic at run time so we can 
				change it dynamicaly, if required
				*/
				AnyNode::DecisionLogic insertion_logic_
			)
				: node_to_be_inserted_( new_node ),
				insertion_logic ( insertion_logic_ )
			{
			}

			/*
			this is the visitor function to be given to 
			tree traversal methods
			*/
			bool operator ()( AnyNode & n) {
				typename AnyNode::where_next leader = insertion_logic(n, node_to_be_inserted_);
					return n.add(leader, node_to_be_inserted_);
			}
		};

	} // anyvisitors

} // dbj

#ifdef DBJ_TESTING_EXISTS
#pragma region dbj testing
namespace tree_testing {

	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": value ptr ")) {

//		AnyNode root = AnyNode( "root payload", "root", AnyNode( "left payload", "L" ), AnyNode( "right payload", "R" )	);

		dbj::AnyNode root{ dbj::make_any(42) };

			 auto printer = [] (AnyNode & n) -> bool {
				 dbj::print("(name: '", n.name, "', uid:", n.uid, ", data: ",  dbj::any_val(n.data).val() , ")");
				 return true;
			 };

			 dbj::print("\n");
			 dbj::preorder(root, printer);
			 dbj::print("\n");
	}
}
#pragma endregion
#endif