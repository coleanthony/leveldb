// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Log format information shared by reader and writer.
// See ../doc/log_format.md for more detail.

#ifndef STORAGE_LEVELDB_DB_LOG_FORMAT_H_
#define STORAGE_LEVELDB_DB_LOG_FORMAT_H_

namespace leveldb {
namespace log {

//日志文件 (*.log) 存储最近update的序列。 每次更新都追加到当前日志文件。 当日志文件达到预定大小时（默认约 4MB），转换为SST（见下文）并为将来的更新创建一个新的日志文件。
//当前日志文件的副本保存在内存结构中（memtable）。 每次读取时都会查询此副本，以便读取操作反映所有记录的更新。

//在打开数据库以及compact之后，会将不再使用的文件删除，使用的函数是RemoveObsoleteFiles
//我们可以在打开leveldb的日志系统，并且适当的在源码中添加日志，此外也可以结合gdb来查看栈调用情况。
//数据库刚打开
//数据库压缩时
//...

//record不是定长的，因为数据长度不一样。
enum RecordType {
  // Zero is reserved for preallocated files
  kZeroType = 0,

  kFullType = 1,

  // For fragments
  kFirstType = 2,
  kMiddleType = 3,
  kLastType = 4
};
static const int kMaxRecordType = kLastType;

//wal日志按照block为单位进行存储的，每个block大小为32k。而每个block是由一系列record组成的。
static const int kBlockSize = 32768;

// Header is checksum (4 bytes), length (2 bytes), type (1 byte).
static const int kHeaderSize = 4 + 2 + 1;

}  // namespace log
}  // namespace leveldb

#endif  // STORAGE_LEVELDB_DB_LOG_FORMAT_H_
