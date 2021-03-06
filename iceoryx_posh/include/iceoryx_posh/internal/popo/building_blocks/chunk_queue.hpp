// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "iceoryx_posh/internal/mepoo/shared_chunk.hpp"
#include "iceoryx_posh/internal/popo/building_blocks/chunk_queue_data.hpp"
#include "iceoryx_utils/cxx/optional.hpp"


namespace iox
{
namespace popo
{
enum class ChunkQueueError
{
    SEMAPHORE_ALREADY_SET
};

/// @brief The ChunkQueue is the low layer building block to receive SharedChunks. It follows a first-in-first-out
/// principle. Together with the ChunkDistributor, the ChunkQueue builds the infrastructure to exchange memory chunks
/// between different data producers and consumers that could be located in different processes. A ChunkQueue is used
/// to build elements of higher abstraction layers that also do memory managemet and provide an API towards the real
/// user
class ChunkQueue
{
  public:
    using MemberType_t = ChunkQueueData;

    ChunkQueue(MemberType_t* const chunkQueueDataPtr) noexcept;

    ChunkQueue(const ChunkQueue& other) = delete;
    ChunkQueue& operator=(const ChunkQueue&) = delete;
    ChunkQueue(ChunkQueue&& rhs) = default;
    ChunkQueue& operator=(ChunkQueue&& rhs) = default;
    ~ChunkQueue() = default;

    /// @brief push a new chunk to the chunk queue
    /// @param[in] shared chunk object
    /// @return if the values was pushed successfully into the chunk queue it returns
    ///         true, otherwise false
    bool push(mepoo::SharedChunk chunk) noexcept;

    /// @brief pop a chunk from the chunk queue
    /// @return optional for a shared chunk that is set if the queue is not empty
    cxx::optional<mepoo::SharedChunk> pop() noexcept;

    /// @brief pop a chunk from the chunk queue
    /// @return if the queue is empty return true, otherwise false
    bool empty() noexcept;

    /// @brief get the current size of the queue. Caution, another thread can have changed the size just after reading
    /// it
    /// @return queue size
    uint64_t size() noexcept;

    /// @brief set the capacity of the queue
    /// @param[in] newCapacity valid values are 0 < newCapacity < MAX_RECEIVER_QUEUE_CAPACITY
    /// @pre it is important that no pop or push calls occur during
    ///         this call
    /// @concurrent not thread safe
    void setCapacity(const uint32_t newCapacity) noexcept;

    /// @brief get the capacity of the queue.
    /// @return queue size
    uint64_t capacity() noexcept;

    /// @brief clear the queue
    void clear() noexcept;

    /// @brief Attach a semaphore which gets triggered if a new chunk is pushed to the queue. Caution, a semaphore
    /// cannot be detached or set again
    /// @param[in] semaphore to attach
    /// @return success if semaphore could be attached, error if not
    cxx::expected<ChunkQueueError> attachSemaphore(mepoo::SharedPointer<posix::Semaphore>) noexcept;

    /// @brief returns the information whether a semaphore is attached. Caution, a semaphore cannot be detached
    /// @return true if the semaphore is set, false if not
    bool isSemaphoreAttached() noexcept;

  protected:
    const MemberType_t* getMembers() const noexcept;
    MemberType_t* getMembers() noexcept;

  private:
    MemberType_t* m_chunkQueueDataPtr{nullptr};
};

} // namespace popo
} // namespace iox
