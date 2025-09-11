// SPDX-License-Identifier: MIT
/**
 * @file garbage_collector.h
 * @author lazypaws
 * @brief Defines Garbage Collector for MeowScript
 * @copyright Copyright(c) 2025 LazyPaws
 */

#pragma once

namespace meow::runtime { 
    struct MeowState;
}

namespace meow::memory {
    struct MeowState;
    struct MeowObject;

    /**
     * @class GarbageCollector
     * @brief Collects unused objects to avoid memory leak
     */
    class GarbageCollector {
    public:
        virtual ~GarbageCollector() = default;
        
        /**
         * @brief Registers an to be tracked by the collector
         * @param[in] object A pointer to the object to be managed
         */
        virtual void registerObject(MeowObject* object) = 0;
        

        /**
         * @brief Runs a garbage collection cycle to free unused objects
         * @param[in] state The current MeowVM state, used to identify root objects for marking
         */
        virtual void collect(meow::runtime::MeowState& state) = 0;
    };
}