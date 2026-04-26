//
#pragma once

#include "lwHeader.h"
#include "lwMath.h"
#include "lwITypes.h"
#include "lwInterfaceExt.h"

LW_BEGIN
	class lwTreeNode : public lwITreeNode {
		LW_STD_DECLARATION()

	private:
		void* _data;
		lwITreeNode* _parent;
		lwITreeNode* _child;
		lwITreeNode* _sibling; // root node has not sibling

	public:
		lwTreeNode();
		~lwTreeNode();

		LW_RESULT EnumTree(lwTreeNodeEnumProc proc, void* param, DWORD enum_type);
		LW_RESULT InsertChild(lwITreeNode* prev_node, lwITreeNode* node);
		LW_RESULT RemoveChild(lwITreeNode* node);
		lwITreeNode* FindNode(lwITreeNode* node);
		lwITreeNode* FindNode(void* data);

		void SetParent(lwITreeNode* node) {
			_parent = node;
		}

		void SetChild(lwITreeNode* node) {
			_child = node;
		}

		void SetSibling(lwITreeNode* node) {
			_sibling = node;
		}

		void SetData(void* data) {
			_data = data;
		}

		lwITreeNode* GetParent() {
			return _parent;
		}

		lwITreeNode* GetChild() {
			return _child;
		}

		lwITreeNode* GetChild(DWORD id);

		lwITreeNode* GetSibling() {
			return _sibling;
		}

		void* GetData() {
			return _data;
		}

		DWORD GetNodeNum() const;
		DWORD GetChildNum() const;
		DWORD GetDepthLevel() const;
	};

LW_END
