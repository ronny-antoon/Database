#ifndef MOCKING_CLASS_HPP
#define MOCKING_CLASS_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// Mock class for NVSDelegateInterface
class MockNVSDelegate : public NVSDelegateInterface
{
public:
    MOCK_METHOD(NVSDelegateError_t, open, (const char *name, NVSDelegateOpenMode_t open_mode, NVSDelegateHandle_t *out_handle), (override));
    MOCK_METHOD(void, close, (NVSDelegateHandle_t handle), (override));
    MOCK_METHOD(NVSDelegateError_t, set_str, (NVSDelegateHandle_t handle, const char *key, const char *value), (override));
    MOCK_METHOD(NVSDelegateError_t, get_str, (NVSDelegateHandle_t handle, const char *key, char *out_value, size_t *max_out_value_length), (override));
    MOCK_METHOD(NVSDelegateError_t, erase_key, (NVSDelegateHandle_t handle, const char *key), (override));
    MOCK_METHOD(NVSDelegateError_t, erase_all, (NVSDelegateHandle_t handle), (override));
};

#endif // MOCKING_CLASS_HPP