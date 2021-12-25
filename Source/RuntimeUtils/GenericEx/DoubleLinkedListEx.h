#pragma once

template <class NodeType, class ElementType>
class TDoubleLinkedListExIterator
{
public:

	explicit TDoubleLinkedListExIterator(NodeType* StartingNode)
		: CurrentNode(StartingNode)
	{ }

	/** conversion to "bool" returning true if the iterator is valid. */
	FORCEINLINE explicit operator bool() const
	{ 
		return CurrentNode != nullptr; 
	}

	TDoubleLinkedListExIterator& operator++()
	{
		checkSlow(CurrentNode);
		CurrentNode = CurrentNode->GetNextNode();
		return *this;
	}

	TDoubleLinkedListExIterator operator++(int)
	{
		auto Tmp = *this;
		++(*this);
		return Tmp;
	}

	TDoubleLinkedListExIterator& operator--()
	{
		checkSlow(CurrentNode);
		CurrentNode = CurrentNode->GetPrevNode();
		return *this;
	}

	TDoubleLinkedListExIterator operator--(int)
	{
		auto Tmp = *this;
		--(*this);
		return Tmp;
	}

	// Accessors.
	ElementType& operator->() const
	{
		checkSlow(CurrentNode);
		return CurrentNode->GetValue();
	}

	ElementType& operator*() const
	{
		checkSlow(CurrentNode);
		return CurrentNode->GetValue();
	}

	NodeType* GetNode() const
	{
		checkSlow(CurrentNode);
		return CurrentNode;
	}

private:
	NodeType* CurrentNode;

	friend bool operator==(const TDoubleLinkedListExIterator& Lhs, const TDoubleLinkedListExIterator& Rhs) { return Lhs.CurrentNode == Rhs.CurrentNode; }
	friend bool operator!=(const TDoubleLinkedListExIterator& Lhs, const TDoubleLinkedListExIterator& Rhs) { return Lhs.CurrentNode != Rhs.CurrentNode; }
};


/**
 * Double linked list.
 */
template <class ElementType>
class TDoubleLinkedListEx
{
public:
	class TDoubleLinkedListExNode
	{
	public:
		friend class TDoubleLinkedListEx;

		/** Constructor */
		TDoubleLinkedListExNode( const ElementType& InValue )
			: Value(InValue), NextNode(nullptr), PrevNode(nullptr), List(nullptr)
		{ }

		const ElementType& GetValue() const
		{
			return Value;
		}

		ElementType& GetValue()
		{
			return Value;
		}

		TDoubleLinkedListExNode* GetNextNode()
		{
			return NextNode;
		}

		const TDoubleLinkedListExNode* GetNextNode() const
		{
			return NextNode;
		}

		TDoubleLinkedListExNode* GetPrevNode()
		{
			return PrevNode;
		}

		const TDoubleLinkedListExNode* GetPrevNode() const
		{
			return PrevNode;
		}

		TDoubleLinkedListEx<ElementType>* GetList()
		{
			return List;
		}

		void RemoveSelf()
		{
			if (List)
				List->RemoveNode(this, false);
		}

	protected:
		ElementType            Value;
		TDoubleLinkedListExNode* NextNode;
		TDoubleLinkedListExNode* PrevNode;
		TDoubleLinkedListEx<ElementType>* List;
	};

	/**
	 * Used to iterate over the elements of a linked list.
	 */
	typedef TDoubleLinkedListExIterator<TDoubleLinkedListExNode,       ElementType> TIterator;
	typedef TDoubleLinkedListExIterator<TDoubleLinkedListExNode, const ElementType> TConstIterator;

	/** Constructors. */
	TDoubleLinkedListEx()
		: HeadNode(nullptr)
		, TailNode(nullptr)
		, ListSize(0)
	{ }

	/** Destructor */
	virtual ~TDoubleLinkedListEx()
	{
		Empty();
	}

	// Adding/Removing methods

	/**
	 * Add the specified value to the beginning of the list, making that value the new head of the list.
	 *
	 * @param	InElement	the value to add to the list.
	 * @return	whether the node was successfully added into the list.
	 * @see GetHead, InsertNode, RemoveNode
	 */
	bool AddHead( const ElementType& InElement )
	{
		return AddHead(new TDoubleLinkedListExNode(InElement));
	}

	bool AddHead( TDoubleLinkedListExNode* NewNode )
	{
		if (NewNode == nullptr)
		{
			return false;
		}

		NewNode->RemoveSelf();
		
		// have an existing head node - change the head node to point to this one
		if ( HeadNode != nullptr )
		{
			NewNode->NextNode = HeadNode;
			HeadNode->PrevNode = NewNode;
			HeadNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		NewNode->List = this;
		SetListSize(ListSize + 1);
		return true;
	}

	/**
	 * Append the specified value to the end of the list
	 *
	 * @param	InElement	the value to add to the list.
	 * @return	whether the node was successfully added into the list
	 * @see GetTail, InsertNode, RemoveNode
	 */
	bool AddTail( const ElementType& InElement )
	{
		return AddTail(new TDoubleLinkedListExNode(InElement));
	}

	__declspec(noinline) bool AddTail( TDoubleLinkedListExNode* NewNode )
	{
		if ( NewNode == nullptr )
		{
			return false;
		}

		NewNode->RemoveSelf();

		if ( TailNode != nullptr )
		{
			TailNode->NextNode = NewNode;
			NewNode->PrevNode = TailNode;
			TailNode = NewNode;
		}
		else
		{
			HeadNode = TailNode = NewNode;
		}

		NewNode->List = this;
		SetListSize(ListSize + 1);
		return true;
	}

	/**
	 * Insert the specified value into the list at an arbitrary point.
	 *
	 * @param	InElement			the value to insert into the list
	 * @param	NodeToInsertBefore	the new node will be inserted into the list at the current location of this node
	 *								if nullptr, the new node will become the new head of the list
	 * @return	whether the node was successfully added into the list
	 * @see Empty, RemoveNode
	 */
	bool InsertNode( const ElementType& InElement, TDoubleLinkedListExNode* NodeToInsertBefore=nullptr )
	{
		return InsertNode(new TDoubleLinkedListExNode(InElement), NodeToInsertBefore);
	}

	bool InsertNode( TDoubleLinkedListExNode* NewNode, TDoubleLinkedListExNode* NodeToInsertBefore=nullptr )
	{
		if ( NewNode == nullptr )
		{
			return false;
		}

		NewNode->RemoveSelf();

		if ( NodeToInsertBefore == nullptr || NodeToInsertBefore == HeadNode )
		{
			return AddHead(NewNode);
		}

		NewNode->PrevNode = NodeToInsertBefore->PrevNode;
		NewNode->NextNode = NodeToInsertBefore;
		NewNode->List = this;

		NodeToInsertBefore->PrevNode->NextNode = NewNode;
		NodeToInsertBefore->PrevNode = NewNode;

		SetListSize(ListSize + 1);
		return true;
	}

	/**
	 * Remove the node corresponding to InElement.
	 *
	 * @param InElement The value to remove from the list.
	 * @see Empty, InsertNode
	 */
	void RemoveNode( const ElementType& InElement )
	{
		TDoubleLinkedListExNode* ExistingNode = FindNode(InElement);
		RemoveNode(ExistingNode);
	}

	/**
	 * Removes the node specified.
	 *
	 * @param NodeToRemove The node to remove.
	 * @param bDeleteNode The node be Destroy
	 * @see Empty, InsertNode
	 */
	void RemoveNode( TDoubleLinkedListExNode* NodeToRemove, bool bDeleteNode = true )
	{
		if (NodeToRemove == nullptr || NodeToRemove->List != this)
			return;

		if (Num() == 1)
		{
			checkSlow(NodeToRemove == HeadNode);
			if (bDeleteNode)
			{
				Empty(true);
			}
			else
			{
				NodeToRemove->NextNode = NodeToRemove->PrevNode = nullptr;
				HeadNode = TailNode = nullptr;
				NodeToRemove->List = nullptr;
				SetListSize(0);
			}
			return;
		}
		
		if ( NodeToRemove == HeadNode )
		{
			HeadNode = HeadNode->NextNode;
			HeadNode->PrevNode = nullptr;
		}
		else if ( NodeToRemove == TailNode )
		{
			TailNode = TailNode->PrevNode;
			TailNode->NextNode = nullptr;
		}
		else
		{
			NodeToRemove->NextNode->PrevNode = NodeToRemove->PrevNode;
			NodeToRemove->PrevNode->NextNode = NodeToRemove->NextNode;
		}

		if (bDeleteNode)
		{
			delete NodeToRemove;
		}
		else
		{
			NodeToRemove->NextNode = NodeToRemove->PrevNode = nullptr;
			NodeToRemove->List = nullptr;
		}
		
		SetListSize(ListSize - 1);
	}

	/** Removes all nodes from the list. */
	void Empty(bool bDestroy = true)
	{
		TDoubleLinkedListExNode* Node;

		if (bDestroy)
		{
			while ( HeadNode != nullptr )
			{
				Node = HeadNode->NextNode;
				delete HeadNode;
				HeadNode = Node;
			}	
		}
		else
		{
			while ( HeadNode != nullptr )
			{
				Node = HeadNode->NextNode;

				HeadNode->PrevNode = nullptr;
				HeadNode->NextNode = nullptr;
				HeadNode->List = nullptr;
				
				HeadNode = Node;
			}	
		}
		

		HeadNode = TailNode = nullptr;
		SetListSize(0);
	}

	// Accessors.

	/**
	 * Returns the node at the head of the list.
	 *
	 * @return Pointer to the first node.
	 * @see GetTail
	 */
	TDoubleLinkedListExNode* GetHead() const
	{
		return HeadNode;
	}

	/**
	 * Returns the node at the end of the list.
	 *
	 * @return Pointer to the last node.
	 * @see GetHead
	 */
	TDoubleLinkedListExNode* GetTail() const
	{
		return TailNode;
	}

	/**
	 * Finds the node corresponding to the value specified
	 *
	 * @param	InElement	the value to find
	 * @return	a pointer to the node that contains the value specified, or nullptr of the value couldn't be found
	 */
	TDoubleLinkedListExNode* FindNode( const ElementType& InElement )
	{
		TDoubleLinkedListExNode* Node = HeadNode;
		while ( Node != nullptr )
		{
			if ( Node->GetValue() == InElement )
			{
				break;
			}

			Node = Node->NextNode;
		}

		return Node;
	}

	bool Contains( const ElementType& InElement )
	{
		return (FindNode(InElement) != nullptr);
	}

	/**
	 * Returns the number of items in the list.
	 *
	 * @return Item count.
	 */
	int32 Num() const
	{
		return ListSize;
	}

protected:

	/**
	 * Updates the size reported by Num().  Child classes can use this function to conveniently
	 * hook into list additions/removals.
	 *
	 * @param	NewListSize		the new size for this list
	 */
	virtual void SetListSize( int32 NewListSize )
	{
		ListSize = NewListSize;
	}

private:
	TDoubleLinkedListExNode* HeadNode = nullptr;
	TDoubleLinkedListExNode* TailNode = nullptr;
	int32 ListSize = 0;

	TDoubleLinkedListEx(const TDoubleLinkedListEx&);
	TDoubleLinkedListEx& operator=(const TDoubleLinkedListEx&);

	friend TIterator      begin(      TDoubleLinkedListEx& List) { return TIterator     (List.GetHead()); }
	friend TConstIterator begin(const TDoubleLinkedListEx& List) { return TConstIterator(List.GetHead()); }
	friend TIterator      end  (      TDoubleLinkedListEx& List) { return TIterator     (nullptr); }
	friend TConstIterator end  (const TDoubleLinkedListEx& List) { return TConstIterator(nullptr); }
};