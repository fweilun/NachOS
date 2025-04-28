# 周志遠 — 作業系統五個 MicroProjects 總算

- **Spec**：各專案的官方指示說明
- **Report**：我們的實作細節與針對 Spec 問題的回答

---

## 專案一覽表

| MicroProject | 主題 | 核心內容 | 關聯主題 |
|:------------|:-----|:---------|:---------|
| **MP1** | System Call Implementation | 實作檔案系統呼叫（Read/Write/Open/Close）與系統結構理解 | File System, System Calls, Makefile, Paging, Assembly |
| **MP2** | Memory Management for Multiprogramming | Trace NachOS 啟動流程，實作 page table 支援 multiprogramming，處理記憶體不足例外 | Memory Management, Page Table, Address Space, Exception Handling |
| **MP3** | Multi-Level Feedback Queue CPU Scheduling | 實作 MLFQ 排程（L1: Preemptive SJF, L2: Non-Preemptive Priority, L3: Round-Robin），加入 Aging 機制避免 starvation | CPU Scheduling, Process Lifecycle, Priority Aging |
| **MP4** | Enhanced File System and Directory Support | 改善 NachOS 檔案系統，支援 linked-list 儲存結構與子目錄操作（mkdir, ls, tree） | File System Enhancement, Linked List Storage, Directory Management, Linux File Commands |
| **Side** | Pthreads Programming - Producer-Consumer Problem | 實作以 pthreads 實現多總程 Producer/Consumer 模式，加入动態管理 consumer 總數，技術處理事件待命與續點說明 | Pthreads, Multithreading, Synchronization, Producer-Consumer Problem |

---

## 各專案詳述

### MP1 — System Call Implementation
- 實作 NachOS 的基本檔案相關系統呼叫：`Read`、`Write`、`Open`、`Close`
- 修改 NachOS 的 Makefile、paging 系統與底層 assembly code，支援系統呼叫運作
- **Related Topics**：File System、System Calls、Makefile、Paging Mechanism、Assembly

---

### MP2 — Memory Management for Multiprogramming
- Trace NachOS 從核心啟動到使用者程式執行的完整流程，理解 thread lifecycle 與 address translation
- 修改記憶體管理機制，實作 page table，支援 multiprogramming
- 自行新增 `MemoryLimitException`，正確處理記憶體不足例外，並確保釋放資源
- **Related Topics**：Memory Management、Page Table、Address Space、Exception Handling

---

### MP3 — Multi-Level Feedback Queue CPU Scheduling
- 實作三層級 CPU 排程（Multi-Level Feedback Queue, MLFQ）：
  - **L1**：Preemptive Shortest Job First (SJF)
  - **L2**：Non-Preemptive Priority Scheduling
  - **L3**：Round-Robin Scheduling
- 加入 Aging 機制，避免低優先權程式因飢餓（starvation）長期無法執行
- **Related Topics**：CPU Scheduling、Process Lifecycle、Priority Aging

---

### MP4 — Enhanced File System and Directory Support
- 儲存結構由單一直接儲存改為 linked-list 方式，支援更大檔案
- 實作子目錄（subdirectory）功能，並支援 Linux 常用檔案指令如 `mkdir`、`ls`、`tree`
- 強化 paging 系統管理，實作多層目錄結構
- **Related Topics**：File System Enhancement、Linked List Storage、Directory Management、Linux File Commands

---

### Pthreads Programming - Producer-Consumer Problem
- 實作以 pthreads 實現多總程 Producer/Consumer 模式
- 4 個 Producer thread、动態管理 Consumer 數量，根據 Worker Queue 滿程度調整人數
- 實作 thread-safe 待命列表 (TSQueue)，使用 pthread condition variable 避免等待模式的 busy waiting
- **Related Topics**：Pthreads、Multithreading、Synchronization、Producer-Consumer Problem
