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
    MOCK_METHOD(NVSDelegateError_t, open, (char const *const name, NVSDelegateOpenMode_t const open_mode, NVSDelegateHandle_t *out_handle), (const override));
    MOCK_METHOD(void, close, (NVSDelegateHandle_t handle), (const override));
    MOCK_METHOD(NVSDelegateError_t, set_str, (NVSDelegateHandle_t handle, char const *const key, char const *const value), (const override));
    MOCK_METHOD(NVSDelegateError_t, get_str, (NVSDelegateHandle_t handle, char const *const key, char *out_value, size_t *length), (const override));
    MOCK_METHOD(NVSDelegateError_t, erase_key, (NVSDelegateHandle_t handle, char const *const key), (const override));
    MOCK_METHOD(NVSDelegateError_t, erase_all, (NVSDelegateHandle_t handle), (const override));
    MOCK_METHOD(NVSDelegateError_t, erase_flash_all, (), (const override));
    MOCK_METHOD(NVSDelegateError_t, commit, (NVSDelegateHandle_t handle), (const override));
};

#endif // MOCKING_CLASS_HPP