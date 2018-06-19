/*
  Created on 19.06.18.
*/

#pragma once

namespace glez::detail::record
{
class RecordedCommands;
}

namespace glez::record
{

class Record
{
public:
    Record();
    ~Record();

    detail::record::RecordedCommands *commands{ nullptr };
};

}