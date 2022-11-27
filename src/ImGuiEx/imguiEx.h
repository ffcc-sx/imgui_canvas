# pragma once

// TODO: Remove this include.
#include "internal/internal_common.h"
#include "internal/internal_api.h"

//------------------------------------------------------------------------------
# define IMGUI_NODE_EDITOR_VERSION      "0.9.2"
# define IMGUI_NODE_EDITOR_VERSION_NUM  000902
//------------------------------------------------------------------------------
namespace ImGuiEx {
    namespace api {

        struct Style;
        struct EditorContext;

        void SetCurrentEditor(EditorContext *ctx);

        EditorContext *GetCurrentEditor();

        EditorContext *CreateEditor(const Config *config = nullptr);

        void DestroyEditor(EditorContext *ctx);

        const Config &GetConfig(EditorContext *ctx = nullptr);

        Style &GetStyle();

        const char *GetStyleColorName(StyleColor colorIndex);

        void PushStyleColor(StyleColor colorIndex, const ImVec4 &color);

        void PopStyleColor(int count = 1);

        void PushStyleVar(StyleVar varIndex, float value);

        void PushStyleVar(StyleVar varIndex, const ImVec2 &value);

        void PushStyleVar(StyleVar varIndex, const ImVec4 &value);

        void PopStyleVar(int count = 1);

        void Begin(const char *id, const ImVec2 &size = ImVec2(0, 0));

        void End();

        void BeginNode(NodeId id);

        void BeginPin(PinId id, PinKind kind);

        void PinRect(const ImVec2 &a, const ImVec2 &b);

        void PinPivotRect(const ImVec2 &a, const ImVec2 &b);

        void PinPivotSize(const ImVec2 &size);

        void PinPivotScale(const ImVec2 &scale);

        void PinPivotAlignment(const ImVec2 &alignment);

        void EndPin();

        void Group(const ImVec2 &size);

        void EndNode();

        bool BeginGroupHint(NodeId nodeId);

        ImVec2 GetGroupMin();

        ImVec2 GetGroupMax();

        ImDrawList *GetHintForegroundDrawList();

        ImDrawList *GetHintBackgroundDrawList();

        void EndGroupHint();

        // TODO: Add a way to manage node background channels
        ImDrawList *GetNodeBackgroundDrawList(NodeId nodeId);

        bool Link(LinkId id, PinId startPinId, PinId endPinId, const ImVec4 &color = ImVec4(1, 1, 1, 1),
                  float thickness = 1.0f);

        void Flow(LinkId linkId, FlowDirection direction = FlowDirection::Forward);

        bool BeginCreate(const ImVec4 &color = ImVec4(1, 1, 1, 1), float thickness = 1.0f);

        bool QueryNewLink(PinId *startId, PinId *endId);

        bool QueryNewLink(PinId *startId, PinId *endId, const ImVec4 &color, float thickness = 1.0f);

        bool QueryNewNode(PinId *pinId);

        bool QueryNewNode(PinId *pinId, const ImVec4 &color, float thickness = 1.0f);

        bool AcceptNewItem();

        bool AcceptNewItem(const ImVec4 &color, float thickness = 1.0f);

        void RejectNewItem();

        void RejectNewItem(const ImVec4 &color, float thickness = 1.0f);

        void EndCreate();

        bool BeginDelete();

        bool QueryDeletedLink(LinkId *linkId, PinId *startId = nullptr, PinId *endId = nullptr);

        bool QueryDeletedNode(NodeId *nodeId);

        bool AcceptDeletedItem(bool deleteDependencies = true);

        void RejectDeletedItem();

        void EndDelete();

        void SetNodePosition(NodeId nodeId, const ImVec2 &editorPosition);

        void SetGroupSize(NodeId nodeId, const ImVec2 &size);

        ImVec2 GetNodePosition(NodeId nodeId);

        ImVec2 GetNodeSize(NodeId nodeId);

        void CenterNodeOnScreen(NodeId nodeId);

        void SetNodeZPosition(NodeId nodeId,
                              float z); // Sets node z position, nodes with higher value are drawn over nodes with lower value
        float GetNodeZPosition(NodeId nodeId); // Returns node z position, defaults is 0.0f

        void RestoreNodeState(NodeId nodeId);

        void Suspend();

        void Resume();

        bool IsSuspended();

        bool IsActive();

        bool HasSelectionChanged();

        int GetSelectedObjectCount();

        int GetSelectedNodes(NodeId *nodes, int size);

        int GetSelectedLinks(LinkId *links, int size);

        bool IsNodeSelected(NodeId nodeId);

        bool IsLinkSelected(LinkId linkId);

        void ClearSelection();

        void SelectNode(NodeId nodeId, bool append = false);

        void SelectLink(LinkId linkId, bool append = false);

        void DeselectNode(NodeId nodeId);

        void DeselectLink(LinkId linkId);

        bool DeleteNode(NodeId nodeId);

        bool DeleteLink(LinkId linkId);

        bool HasAnyLinks(NodeId nodeId); // Returns true if node has any link connected
        bool HasAnyLinks(PinId pinId); // Return true if pin has any link connected
        int BreakLinks(NodeId nodeId); // Break all links connected to this node
        int BreakLinks(PinId pinId); // Break all links connected to this pin

        void NavigateToContent(float duration = -1);

        void NavigateToSelection(bool zoomIn = false, float duration = -1);

        bool ShowNodeContextMenu(NodeId *nodeId);

        bool ShowPinContextMenu(PinId *pinId);

        bool ShowLinkContextMenu(LinkId *linkId);

        bool ShowBackgroundContextMenu();

        void EnableShortcuts(bool enable);

        bool AreShortcutsEnabled();

        bool BeginShortcut();

        bool AcceptCut();

        bool AcceptCopy();

        bool AcceptPaste();

        bool AcceptDuplicate();

        bool AcceptCreateNode();

        int GetActionContextSize();

        int GetActionContextNodes(NodeId *nodes, int size);

        int GetActionContextLinks(LinkId *links, int size);

        void EndShortcut();

        float GetCurrentZoom();

        NodeId GetHoveredNode();

        PinId GetHoveredPin();

        LinkId GetHoveredLink();

        NodeId GetDoubleClickedNode();

        PinId GetDoubleClickedPin();

        LinkId GetDoubleClickedLink();

        bool IsBackgroundClicked();

        bool IsBackgroundDoubleClicked();

        ImGuiMouseButton GetBackgroundClickButtonIndex(); // -1 if none
        ImGuiMouseButton GetBackgroundDoubleClickButtonIndex(); // -1 if none

        bool GetLinkPins(LinkId linkId, PinId *startPinId,
                         PinId *endPinId); // pass nullptr if particular pin do not interest you

        bool PinHadAnyLinks(PinId pinId);

        ImVec2 GetScreenSize();

        ImVec2 ScreenToCanvas(const ImVec2 &pos);

        ImVec2 CanvasToScreen(const ImVec2 &pos);

        // Returns number of submitted nodes since Begin() call
        int GetNodeCount();
        // Fills an array with node id's in order they're drawn; up to 'size` elements are set. Returns actual size of filled id's.
        int GetOrderedNodeIds(NodeId *nodes, int size);

    } // end namespace Editor
} // namespace ax
