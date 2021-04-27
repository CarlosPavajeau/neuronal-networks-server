//
// Created by cantte on 26/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MESSAGE_HPP
#define NEURONAL_NETWORKS_SERVER_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class Message
{
public:
    enum
    {
        op_code_length = 4,
        size_length = 4,
        header_length = op_code_length + size_length
    };
    enum
    {
        max_body_length = 4096
    };

    Message() : _body_length(0) {}

    const char* Data() const
    {
        return _data;
    }

    char* Data()
    {
        return _data;
    }

    std::size_t Length() const
    {
        return header_length + _body_length;
    }

    const char* Body() const
    {
        return _data + header_length;
    }

    char* Body()
    {
        return _data + header_length;
    }

    std::size_t BodyLength() const
    {
        return _body_length;
    }

    void BodyLength(std::size_t new_length)
    {
        _body_length = new_length;
        if (_body_length > max_body_length)
        {
            _body_length = max_body_length;
        }
    }

    std::size_t Opcode() const
    {
        return _op_code;
    }

    void Opcode(std::size_t opcode)
    {
        _op_code = opcode;
    }

    bool DecodeHeader()
    {
        char header[header_length + 1] = "";
        std::strncat(header, _data, header_length);

        char opcode_str[op_code_length + 1] = "";
        std::strncpy(opcode_str, header, op_code_length);
        _op_code = std::atoi(opcode_str);

        char body_size_str[size_length + 1] = "";
        std::strncpy(body_size_str, header + op_code_length, size_length);

        _body_length = std::atoi(body_size_str);
        if (_body_length > max_body_length)
        {
            _body_length = 0;
            return false;
        }

        return true;
    }

    void EncodeHeader()
    {
        char header[header_length + 1] = "";
        std::sprintf(header, "%4d%4d", static_cast<int>(_op_code), static_cast<int>(_body_length));
        std::memcpy(_data, header, header_length);
    }

private:
    char _data[header_length + max_body_length];
    std::size_t _op_code;
    std::size_t _body_length;
};

#endif //NEURONAL_NETWORKS_SERVER_MESSAGE_HPP
