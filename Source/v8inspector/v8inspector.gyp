# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
    'includes': [
        '../wtf/wtf.gypi',
    ],
    'targets': [
        {
            'target_name': 'v8inspector_main',
            'type': 'executable',
            'dependencies': [
                'http_server',
                '../config.gyp:config',
                '../core/core.gyp:webcore_v8inspector',
                '../wtf/wtf.gyp:wtf',
                '<(DEPTH)/v8/tools/gyp/v8.gyp:v8',
                '<(DEPTH)/v8/tools/gyp/v8.gyp:v8_libplatform', # for V8InspectorMain
            ],
            'sources': [
                'remote_debugging_server.cc',
                'remote_debugging_server.h',

                'StreamSocketImpl.cpp',
                'StreamSocketImpl.h',

                'ServerSocketImpl.cpp',
                'ServerSocketImpl.h',


                'InspectorMessageLoop.cpp',
                'InspectorMessageLoop.h',

                'V8InspectorMain.cpp',
                'V8Inspector.cpp',
                'V8Inspector.h',
            ],
            'include_dirs': [
                '..',  # WebKit/Source
                '../chrome',  # WebKit/Source/chrome
                '<(DEPTH)/v8', # for include/v8-platform.h in include/libplatform/libplatform.h
            ],
            'defines': [
                'INSIDE_BLINK',
            ],
        },

        {
          'target_name': 'http_server',
          'type': 'static_library',
          'variables': { 'enable_wexit_time_destructors': 1, },
          'dependencies': [
#            '../chrome/url/url.gyp:url_lib',
            '../chrome/third_party/zlib/zlib.gyp:zlib',
          ],
          'sources': [
            '../chrome/net/server/http_connection.cc',
            '../chrome/net/server/http_connection.h',
            '../chrome/net/server/http_server.cc',
            '../chrome/net/server/http_server.h',
            '../chrome/net/server/http_server_request_info.cc',
            '../chrome/net/server/http_server_request_info.h',
            '../chrome/net/server/http_server_response_info.cc',
            '../chrome/net/server/http_server_response_info.h',
            '../chrome/net/server/web_socket.cc',
            '../chrome/net/server/web_socket.h',
            '../chrome/net/server/web_socket_encoder.cc',
            '../chrome/net/server/web_socket_encoder.h',

            '../chrome/net/socket/stream_socket.cc',
            '../chrome/net/socket/stream_socket.h',
            '../chrome/net/socket/tcp_client_socket.cc',
            '../chrome/net/socket/tcp_client_socket.h',
            '../chrome/net/socket/tcp_server_socket.cc',
            '../chrome/net/socket/tcp_server_socket.h',
            '../chrome/net/socket/tcp_socket.h',
            '../chrome/net/socket/tcp_socket_libevent.cc',
            '../chrome/net/socket/tcp_socket_libevent.h',

            '../chrome/net/websockets/websocket_deflater.cc',
            '../chrome/net/websockets/websocket_deflater.h',
            '../chrome/net/websockets/websocket_inflater.cc',
            '../chrome/net/websockets/websocket_inflater.h',
            '../chrome/net/websockets/websocket_extension.cc',
            '../chrome/net/websockets/websocket_extension.h',
            '../chrome/net/websockets/websocket_extension_parser.cc',
            '../chrome/net/websockets/websocket_extension_parser.h',


            '../chrome/net/base/address_list.cc',
            '../chrome/net/base/address_list.h',
            '../chrome/net/base/io_buffer.cc',
            '../chrome/net/base/io_buffer.h',
            '../chrome/net/base/ip_endpoint.cc',
            '../chrome/net/base/ip_endpoint.h',
            '../chrome/net/base/net_util.cc',
            '../chrome/net/base/net_util.h',

            '../chrome/net/http/http_status_code.cc',
            '../chrome/net/http/http_status_code.h',
            '../chrome/net/http/http_request_headers.cc',
            '../chrome/net/http/http_request_headers.h',

            '../chrome/net/log/net_log.cc',
            '../chrome/net/log/net_log.h',
            '../chrome/net/log/net_log_capture_mode.cc',
            '../chrome/net/log/net_log_capture_mode.h',

            '../chrome/net/socket/socket_libevent.cc',
            '../chrome/net/socket/socket_libevent.h',
            '../chrome/net/socket/socket_descriptor.cc',
            '../chrome/net/socket/socket_descriptor.h',
            '../chrome/net/socket/socket_net_log_params.cc',
            '../chrome/net/socket/socket_net_log_params.h',
            '../chrome/net/base/connection_type_histograms.cc',
            '../chrome/net/base/connection_type_histograms.h',
            '../chrome/net/base/net_errors_posix.cc',
            '../chrome/net/base/net_errors.h',
            '../chrome/net/base/network_activity_monitor.cc',
            '../chrome/net/base/network_activity_monitor.h',
            '../chrome/net/base/network_change_notifier.cc',
            '../chrome/net/base/network_change_notifier.h',
            '../chrome/net/socket/server_socket.cc',
            '../chrome/net/socket/server_socket.h',

            # For Debug build
            '../chrome/net/dns/dns_config_service.cc',
            '../chrome/net/dns/dns_config_service.h',
            '../chrome/net/base/ip_pattern.cc',
            '../chrome/net/base/ip_pattern.h',
            '../chrome/net/http/http_util.cc',
            '../chrome/net/http/http_util.h',
            '../chrome/net/http/http_byte_range.cc',
            '../chrome/net/http/http_byte_range.h',
            '../chrome/net/base/network_change_notifier_linux.cc',
            '../chrome/net/base/network_change_notifier_linux.h',
            '../chrome/net/base/escape.cc',
            '../chrome/net/base/escape.h',
            '../chrome/net/base/address_tracker_linux.cc',
            '../chrome/net/base/address_tracker_linux.h',
            '../chrome/net/base/net_util_linux.cc',
            '../chrome/net/base/net_util_linux.h',
            '../chrome/net/base/net_util_posix.cc',
            '../chrome/net/base/net_util_posix.h',
            '../chrome/net/dns/dns_config_service_posix.cc',
            '../chrome/net/dns/dns_config_service_posix.h',
            '../chrome/net/dns/serial_worker.cc',
            '../chrome/net/dns/serial_worker.h',
            '../chrome/net/dns/dns_hosts.cc',
            '../chrome/net/dns/dns_hosts.h',
            '../chrome/net/http/http_log_util.cc',
            '../chrome/net/http/http_log_util.h',
            '../chrome/net/http/http_auth_challenge_tokenizer.cc',
            '../chrome/net/http/http_auth_challenge_tokenizer.h',
            '../chrome/net/base/host_port_pair.cc',
            '../chrome/net/base/host_port_pair.h',
            '../chrome/net/base/dns_util.cc',
            '../chrome/net/base/dns_util.h',
          ],
          'include_dirs': [
            '../chrome',  # WebKit/Source/chrome
          ],
          # TODO(jschuh): crbug.com/167187 fix size_t to int truncations.
          'msvs_disabled_warnings': [4267, ],
        },

    ],
}
