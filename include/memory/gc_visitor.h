// SPDX-License-Identifier: MIT
/**
 * @file gc_visitor.h
 * @author lazypaws
 * @brief Defines GCVisitor for MeowScript
 * @copyright Copyright(c) 2025 LazyPaws
 */

#pragma once

namespace meow::memory {
    struct MeowObject;

    /**
     * @struct GCVisitor
     * @brief Used by objects to visit value or object related to
     */
    struct GCVisitor {
        virtual ~GCVisitor() = default;

        /**
         * @brief Visits the given value
         * @param[in] value The value to be visited
         */
        virtual void visitValue(meow::common::Value& value) = 0;

        /**
         * @brief Visits the given object
         * @param[in] object A pointer to the object to be visited
         */
        virtual void visitObject(MeowObject* object) = 0;
    };
}