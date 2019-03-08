# Benchmark

This document records the benchmark results.<br/>
Environment: VMware Player on i5-6400 2.7GHz.<br/>

## Select

Task: select 100 records from 200 records 10000 times (single thread).

| Driver                           | Cores | Time samples (seconds)                     | Average time |
|----------------------------------|-------|--------------------------------------------|--------------|
| DataStax C++ 2.8.1               | 1     | `[17.768, 19.647, 18.107, 19.646, 19.818]` | 18.997       |
| DataStax C++ 2.8.1 (+Prepare)    | 1     | `[17.734, 17.590, 19.719, 19.857, 18.064]` | 18.592       |
| DataStax Python 3.13             | 1     | `[29.748, 29.028, 30.754, 31.047, 31.352]` | 30.385       |
| DataStax Python 3.13 (+Prepare)  | 1     | `[29.747, 31.884, 27.989, 29.595, 31.187]` | 30.080       |
| DataStax Python 3.13 (+Compress) | 1     | `[31.395, 33.454, 29.540, 32.949, 32.495]` | 31.966       |
| CQLDriver 0.1                    | 1     | `[19.575, 21.862, 20.244, 19.889, 20.048]` | 20.323       |
| CQLDriver 0.2                    | 1     | `[19.946, 20.090, 19.979, 19.951, 20.005]` | 19.994       |
| CQLDriver 0.2 (+Prepare)         | 1     | `[19.737, 19.840, 19.548, 19.761, 19.734]` | 19.724       |
| CQLDriver 0.2 (+Compress)        | 1     | `[19.752, 19.777, 19.941, 19.803, 19.822]` | 19.819       |
| CQLDriver 0.3                    | 1     | `[25.033, 25.031, 24.750, 25.206, 25.037]` | 25.011       |
| CQLDriver 0.3 (+Prepare)         | 1     | `[25.075, 24.788, 25.075, 25.357, 24.593]` | 24.977       |
| CQLDriver 0.3 (+Compress)        | 1     | `[25.899, 25.498, 25.471, 25.351, 25.404]` | 25.524       |

Task: select 100 records from 200 records 20000 times (multiple threads with concurrent tasks).

| Driver                           | Cores | Time samples (seconds)                     | Average time |
|----------------------------------|-------|--------------------------------------------|--------------|
| DataStax C++ 2.8.1 (+Prepare)    | 4*20  | `[11.161, 11.644, 11.228, 11.079, 11.137]` | 11.249       |
| CQLDriver 0.3 (+Prepare)         | 4*20  | `[9.373, 9.787, 9.784, 9.770, 9.600]`      | 9.662        |

## Insert

Task: insert a single record 10000 times (single thread).

| Driver                           | Cores | Time samples (seconds)                 | Average time |
|----------------------------------|-------|----------------------------------------|--------------|
| DataStax C++ 2.8.1               | 1     | `[4.617, 5.095, 4.527, 5.071, 4.519]`  | 4.765        |
| DataStax C++ 2.8.1 (+Prepare)    | 1     | `[4.023, 4.298, 4.469, 4.484, 4.036]`  | 4.262        |
| DataStax Python 3.13             | 1     | `[8.868, 8.464, 8.959, 10.033, 9.464]` | 9.157        |
| DataStax Python 3.13 (+Prepare)  | 1     | `[8.495, 9.096, 8.891, 9.696, 8.893]`  | 9.014        |
| DataStax Python 3.13 (+Compress) | 1     | `[8.926, 9.419, 8.493, 8.614, 9.370]`  | 8.964        |
| CQLDriver 0.1                    | 1     | `[6.364, 6.539, 6.454, 6.386, 6.565]`  | 6.461        |
| CQLDriver 0.2                    | 1     | `[6.334, 6.654, 6.654, 6.401, 6.388]`  | 6.486        |
| CQLDriver 0.2 (+Prepare)         | 1     | `[6.162, 6.082, 6.008, 6.104, 6.112]`  | 6.093        |
| CQLDriver 0.2 (+Compress)        | 1     | `[6.602, 6.650, 6.575, 6.381, 6.779]`  | 6.597        |
| CQLDriver 0.3                    | 1     | `[6.417, 6.479, 6.535, 6.419, 6.344]`  | 6.438        |
| CQLDriver 0.3 (+Prepare)         | 1     | `[6.409, 6.133, 5.943, 6.226, 6.244]`  | 6.191        |
| CQLDriver 0.3 (+Compress)        | 1     | `[6.549, 6.483, 6.739, 6.655, 6.590]`  | 6.603        |

Task: insert a single record 100000 times (multiple threads with concurrent tasks).

| Driver                           | Cores | Time samples (seconds)                     | Average time |
|----------------------------------|-------|--------------------------------------------|--------------|
| DataStax C++ 2.8.1 (+Prepare)    | 4*20  | `[4.812, 4.496, 4.347, 4.770, 4.270]`      | 4.539        |
| CQLDriver 0.3 (+Prepare)         | 4*20  | `[3.614, 3.680, 3.385, 3.699, 3.702]`      | 3.615        |

## Batch Insert

Task: batch insert 100 records 1000 times (single thread).

| Driver                           | Cores | Time samples (seconds)                     | Average time |
|----------------------------------|-------|--------------------------------------------|--------------|
| DataStax C++ 2.8.1               | 1     | `[6.598, 5.597, 5.319, 6.441, 6.146]`      | 6.020        |
| DataStax C++ 2.8.1 (+Prepare)    | 1     | `[5.357, 4.341, 5.306, 4.690, 5.587]`      | 5.056        |
| DataStax Python 3.13             | 1     | `[56.234, 55.419, 56.557, 56.560, 56.500]` | 56.254       |
| DataStax Python 3.13 (+Prepare)  | 1     | `[7.032, 6.922, 7.757, 8.546, 8.377]`      | 7.726        |
| DataStax Python 3.13 (+Compress) | 1     | `[56.648, 56.958, 55.662, 55.957, 56.357]` | 56.316       |
| DataStax Python 3.13 (+P+C)      | 1     | `[4.946, 6.296, 5.906, 5.323, 5.465]`      | 5.587        |
| CQLDriver 0.1                    | 1     | `[5.051, 5.486, 6.631, 6.788, 6.258]`      | 6.042        |
| CQLDriver 0.2                    | 1     | `[6.004, 6.416, 6.559, 7.909, 6.321]`      | 6.641        |
| CQLDriver 0.2 (+Prepare)         | 1     | `[4.795, 4.982, 5.063, 5.050, 5.587]`      | 5.095        |
| CQLDriver 0.2 (+Compress)        | 1     | `[5.474, 5.233, 5.177, 5.079, 5.591]`      | 5.310        |
| CQLDriver 0.2 (+P+C)             | 1     | `[3.574, 4.466, 4.604, 4.495, 4.372]`      | 4.302        |
| CQLDriver 0.3                    | 1     | `[5.129, 5.518, 7.443, 6.999, 5.754]`      | 6.168        |
| CQLDriver 0.3 (+Prepare)         | 1     | `[4.455, 4.305, 4.616, 4.376, 5.052]`      | 4.560        |
| CQLDriver 0.3 (+Compress)        | 1     | `[5.146, 5.634, 5.887, 5.126, 4.500]`      | 5.258        |
| CQLDriver 0.3 (+P+C)             | 1     | `[3.662, 5.279, 4.225, 4.138, 4.767]`      | 4.414        |

Task: batch insert 100 records 10000 times (multiple threads with concurrent tasks).

| Driver                           | Cores | Time samples (seconds)                     | Average time |
|----------------------------------|-------|--------------------------------------------|--------------|
| DataStax C++ 2.8.1 (+Prepare)    | 4*20  | `[33.697, 31.958, 30.513, 30.336, 33.915]` | 32.083       |
| CQLDriver 0.3 (+Prepare)         | 4*20  | `[30.219, 28.663, 28.507, 27.618, 26.074]` | 28.216       |
| CQLDriver 0.3 (+P+C)             | 4*20  | `[24.043, 22.687, 23.060, 23.902, 21.607]` | 23.059       |

## Performance analysis (live-profiler)

Provided by [live-profiler](https://github.com/cpv-project/live-profiler).<br/>
Last updated version: 0.2<br/>
Last analysis target: batch insert +p+c single core<br/>

### CpuSampleFrequencyAnalyzer

``` text
collect for benchmark in 180000 ms
top 100 inclusive symbol names:
No. Overhead Samples SymbolName
  1     0.23    1270 epoll_pwait
  2     0.18     989 brnf_frag_data_storage
  3     0.13     703 seastar::reactor_backend_epoll::~reactor_backend_epoll()
  4     0.09     511 __clock_gettime
  5     0.07     370 seastar::smp::poll_queues()
  6     0.05     296 std::_Function_handler<bool (), seastar::reactor::run()::{lambda()#7}>::_M_invoke(std::_Any_data const&)
  7     0.04     211 seastar::reactor::run()
  8     0.03     184 seastar::smp_message_queue::flush_response_batch()
  9     0.03     143 unsigned long seastar::smp_message_queue::process_queue<4ul, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}>(seastar::smp_message_queue::lf_queue&, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}) [clone .isra.3534]
 10     0.02     140 seastar::internal::io_getevents(unsigned long, long, long, io_event*, timespec const*)
 11     0.02     135 unsigned long seastar::smp_message_queue::process_queue<2ul, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}>(seastar::smp_message_queue::lf_queue&, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}) [clone .isra.3535]
 12     0.02     108 pthread_sigmask
 13     0.02     101 seastar::reactor::smp_pollfn::~smp_pollfn()
 14     0.02     100 seastar::reactor::signals::action(int, siginfo_t*, void*)
 15     0.02      93 timerfd_settime
 16     0.02      90 seastar::reactor::process_io()
 17     0.01      76 seastar::basic_semaphore<seastar::semaphore_default_exception_factory, std::chrono::_V2::steady_clock>::signal(unsigned long) [clone .part.3035]
 18     0.01      76 seastar::reactor::sleep()
 19     0.01      68 seastar::reactor_backend_epoll::wait_and_process(int, __sigset_t const*)
 20     0.01      50 seastar::reactor::io_pollfn::poll()
 21     0.01      50 seastar::reactor::flush_pending_aio()
 22     0.01      47 seastar::syscall_work_queue::complete()
 23     0.01      46 cql::Object<cql::BatchCommandData>::Object(std::unique_ptr<cql::BatchCommandData, std::default_delete<cql::BatchCommandData> >&&)::{lambda(void*)#1}::_FUN(void*)
 24     0.01      45 seastar::reactor::execution_stage_pollfn::poll()
 25     0.01      42 seastar::reactor::do_expire_lowres_timers()
 26     0.01      41 _ZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSA_3_V212system_clockENSA_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS1_S3_S5_ENKUlvE2_clEvENKUlvE_clEv
 27     0.01      40 std::chrono::_V2::steady_clock::now()
 28     0.01      37 __mprotect
 29     0.01      34 LZ4_compress_fast_extState
 30     0.01      31 seastar::reactor::flush_tcp_batches()
 31     0.01      30 seastar::memory::cpu_pages::free(void*)
 32     0.01      30 seastar::memory::drain_cross_cpu_freelist()
 33     0.01      29 cql::Connection::encodeMessage(cql::Object<cql::RequestMessageBase> const&)
 34     0.00      28 _ZZZN3cql7Session12batchExecuteEONS_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS1_N7seastar13lw_shared_ptrINS_14ConnectionPoolEEENS_12_GLOBAL__N_19RetryFlowENSK_INS_10ConnectionEEENS_16ConnectionStreamENS_6ObjectINS_12BatchMessageEEENSS_INSN_14PrepareResultsEEEEEDaS4_S6_S8_SA_SC_SE_SG_ENKUlvE_clEv
 35     0.00      28 seastar::reactor::signals::poll_signal()
 36     0.00      26 seastar::systemwide_memory_barrier()
 37     0.00      24 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace_aux(unsigned long, unsigned long, unsigned long, char)
 38     0.00      22 cql::BatchCommand::openParameterSet() &
 39     0.00      21 __errno_location
 40     0.00      20 __getpagesize
 41     0.00      20 cql::BatchMessage::encodeBody(cql::ConnectionInfo const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) const
 42     0.00      20 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_mutate(unsigned long, unsigned long, char const*, unsigned long)
 43     0.00      19 std::_Function_handler<void (), seastar::reactor::run()::{lambda()#5}>::_M_invoke(std::_Any_data const&)
 44     0.00      19 seastar::reactor::syscall_pollfn::poll()
 45     0.00      19 seastar::reactor::del_timer(seastar::timer<std::chrono::_V2::steady_clock>*)
 46     0.00      19 cql::Connection::sendMessage(cql::Object<cql::RequestMessageBase>&&, cql::ConnectionStream const&)
 47     0.00      17 seastar::reactor::aio_batch_submit_pollfn::poll()
 48     0.00      16 seastar::reactor::lowres_timer_pollfn::exit_interrupt_mode()
 49     0.00      16 __tls_init
 50     0.00      16 seastar::reactor::batch_flush_pollfn::poll()
 51     0.00      15 timer_settime
 52     0.00      15 seastar::try_systemwide_memory_barrier()
 53     0.00      13 seastar::reactor::epoll_pollfn::poll()
 54     0.00      13 seastar::reactor::epoll_pollfn::exit_interrupt_mode()
 55     0.00      13 seastar::thread::try_run_one_yielded_thread()
 56     0.00      12 seastar::memory::cpu_pages::allocate_small(unsigned int)
 57     0.00      11 seastar::reactor::drain_cross_cpu_freelist_pollfn::poll()
 58     0.00      11 seastar::memory::small_pool::allocate()
 59     0.00      10 seastar::reactor::lowres_timer_pollfn::poll()
 60     0.00       9 _ZN7seastar8futurizeINS_6futureIJNS_16temporary_bufferIcEEEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS7_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS8_EENS7_6ObjectINS7_19ResponseMessageBaseEEEEEDaSD_SF_ENKUlvE_clEvEUlOS3_E_JS3_EEES4_OSC_OSt5tupleIJDpT0_EE
 61     0.00       9 _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlvE0_JEEES2_OS9_OSt5tupleIJDpT0_EE.isra.704
 62     0.00       9 LZ4_decompress_safe
 63     0.00       9 seastar::reactor::add_task(std::unique_ptr<seastar::task, std::default_delete<seastar::task> >&&)
 64     0.00       8 void seastar::promise<>::set_value<>()
 65     0.00       8 seastar::memory::free(void*, unsigned long)
 66     0.00       8 _ZN7seastar8internal8repeaterIZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSD_3_V212system_clockENSD_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS4_S6_S8_ENKUlvE2_clEvEUlvE_E15run_and_disposeEv
 67     0.00       8 cql::BatchCommand::BatchCommand()
 68     0.00       8 _ZN7seastar8futurizeINS_10bool_classINS_18stop_iteration_tagEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_ENKUlvE_clEvEUlONS_16temporary_bufferIcEEE0_JSO_EEENS_6futureIJS3_EEEOSB_OSt5tupleIJDpT0_EE
 69     0.00       7 seastar::memory::cpu_pages::drain_cross_cpu_freelist()
 70     0.00       7 seastar::output_stream<char>::write(char const*, unsigned long)
 71     0.00       7 seastar::free_deleter_impl::~free_deleter_impl()
 72     0.00       7 _ZN7seastar12continuationIZNS_6futureIJEE4thenIZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSF_3_V212system_clockENSF_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS6_S8_SA_ENKUlvE2_clEvENKUlvE_clEvEUlvE_NS1_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEES7_OS5_EUlSV_E_JEE15run_and_disposeEv
 73     0.00       7 seastar::lowres_clock_impl::update()
 74     0.00       7 cql::Connection::waitNextMessage(cql::ConnectionStream const&)
 75     0.00       7 cql::Session::batchExecute(cql::BatchCommand&&)
 76     0.00       6 seastar::memory::allocate(unsigned long)
 77     0.00       6 read
 78     0.00       6 std::_Hash_bytes(void const*, unsigned long, unsigned long)
 79     0.00       6 operator delete(void*, unsigned long)
 80     0.00       6 __send
 81     0.00       6 seastar::future<unsigned long>::forward_to(seastar::promise<unsigned long>&&)
 82     0.00       6 cql::BatchCommand::getParameterCountOfLastSet() &
 83     0.00       6 _ZN7seastar8internal8repeaterIZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSD_3_V212system_clockENSD_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS4_S6_S8_ENKUlvE2_clEvEUlvE_ED0Ev
 84     0.00       6 cql::Connection::decodeMessage(cql::Object<cql::ResponseMessageBase>&, seastar::temporary_buffer<char>&&) const
 85     0.00       6 seastar::reactor::io_pollfn::exit_interrupt_mode()
 86     0.00       5 seastar::reactor::io_pollfn::try_enter_interrupt_mode()
 87     0.00       5 seastar::reactor::signal_pollfn::try_enter_interrupt_mode()
 88     0.00       5 seastar::future<seastar::temporary_buffer<char> > seastar::futurize<seastar::future<seastar::temporary_buffer<char> > >::apply<seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}, seastar::temporary_buffer<char> >(seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}&&, std::tuple<seastar::temporary_buffer<char> >&&)
 89     0.00       5 seastar::reactor::smp_pollfn::poll()
 90     0.00       5 _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedINS5_12finally_bodyIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlvE6_Lb0EEES5_EESE_OSC_EUlS18_E_JS4_EE15run_and_disposeEv
 91     0.00       5 _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedIZNS5_16handle_exceptionIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlNSt15__exception_ptr13exception_ptrEE5_EES5_OSC_EUlS19_E_S5_EESE_S19_EUlS19_E_JS4_EE15run_and_disposeEv
 92     0.00       5 seastar::reactor::syscall_pollfn::exit_interrupt_mode()
 93     0.00       5 seastar::reactor::signal_pollfn::poll()
 94     0.00       5 _ZN7seastar12continuationIZNS_6futureIJN3cql6ObjectINS2_19ResponseMessageBaseEEEEE4thenIZZZNS2_7Session12batchExecuteEONS2_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS9_NS_13lw_shared_ptrINS2_14ConnectionPoolEEENS2_12_GLOBAL__N_19RetryFlowENSR_INS2_10ConnectionEEENS2_16ConnectionStreamENS3_INS2_12BatchMessageEEENS3_INSU_14PrepareResultsEEEEEDaSC_SE_SG_SI_SK_SM_SO_ENKUlvE_clEvEUlSB_E3_NS1_IJEEEEESD_OSB_EUlS16_E_JS5_EE15run_and_disposeEv
 95     0.00       4 seastar::reactor::activate(seastar::reactor::task_queue&) [clone .part.2741]
 96     0.00       4 cql::MessageHeader::MessageHeader()
 97     0.00       4 std::enable_if<!seastar::is_future<std::result_of<seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2} (seastar::future<>&&)>::type>::value, seastar::future<> >::type seastar::do_void_futurize_apply<seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2}, seastar::future<> >(std::result_of&&, seastar::future<>&&)
 98     0.00       4 seastar::memory::free(void*)
 99     0.00       4 seastar::promise<>::abandoned() [clone .isra.195]
100     0.00       4 _ZZZN3cql10Connection15waitNextMessageERKNS_16ConnectionStreamEENKUlRT_RT0_E_clIN7seastar13lw_shared_ptrIS0_EENS_6ObjectINS_19ResponseMessageBaseEEEEEDaS5_S7_ENKUlvE_clEv

top 100 exclusive symbol names:
No. Overhead Samples SymbolName
  1     0.23    1270 epoll_pwait
  2     0.07     370 seastar::smp::poll_queues()
  3     0.05     296 std::_Function_handler<bool (), seastar::reactor::run()::{lambda()#7}>::_M_invoke(std::_Any_data const&)
  4     0.04     211 seastar::reactor::run()
  5     0.03     184 seastar::smp_message_queue::flush_response_batch()
  6     0.03     143 unsigned long seastar::smp_message_queue::process_queue<4ul, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}>(seastar::smp_message_queue::lf_queue&, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}) [clone .isra.3534]
  7     0.02     140 seastar::internal::io_getevents(unsigned long, long, long, io_event*, timespec const*)
  8     0.02     135 unsigned long seastar::smp_message_queue::process_queue<2ul, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}>(seastar::smp_message_queue::lf_queue&, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}) [clone .isra.3535]
  9     0.02     108 pthread_sigmask
 10     0.02     100 seastar::reactor::signals::action(int, siginfo_t*, void*)
 11     0.02      93 timerfd_settime
 12     0.02      90 seastar::reactor::process_io()
 13     0.01      76 seastar::basic_semaphore<seastar::semaphore_default_exception_factory, std::chrono::_V2::steady_clock>::signal(unsigned long) [clone .part.3035]
 14     0.01      68 seastar::reactor_backend_epoll::wait_and_process(int, __sigset_t const*)
 15     0.01      66 __clock_gettime
 16     0.01      50 seastar::reactor::io_pollfn::poll()
 17     0.01      50 seastar::reactor::flush_pending_aio()
 18     0.01      47 seastar::syscall_work_queue::complete()
 19     0.01      45 seastar::reactor::sleep()
 20     0.01      45 seastar::reactor::execution_stage_pollfn::poll()
 21     0.01      42 seastar::reactor::do_expire_lowres_timers()
 22     0.01      40 std::chrono::_V2::steady_clock::now()
 23     0.01      37 __mprotect
 24     0.01      34 LZ4_compress_fast_extState
 25     0.01      30 seastar::memory::cpu_pages::free(void*)
 26     0.01      30 seastar::memory::drain_cross_cpu_freelist()
 27     0.00      28 _ZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSA_3_V212system_clockENSA_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS1_S3_S5_ENKUlvE2_clEvENKUlvE_clEv
 28     0.00      28 seastar::reactor::signals::poll_signal()
 29     0.00      27 seastar::reactor::flush_tcp_batches()
 30     0.00      26 seastar::systemwide_memory_barrier()
 31     0.00      24 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace_aux(unsigned long, unsigned long, unsigned long, char)
 32     0.00      22 cql::BatchCommand::openParameterSet() &
 33     0.00      21 __errno_location
 34     0.00      20 __getpagesize
 35     0.00      20 _ZZZN3cql7Session12batchExecuteEONS_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS1_N7seastar13lw_shared_ptrINS_14ConnectionPoolEEENS_12_GLOBAL__N_19RetryFlowENSK_INS_10ConnectionEEENS_16ConnectionStreamENS_6ObjectINS_12BatchMessageEEENSS_INSN_14PrepareResultsEEEEEDaS4_S6_S8_SA_SC_SE_SG_ENKUlvE_clEv
 36     0.00      20 cql::BatchMessage::encodeBody(cql::ConnectionInfo const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) const
 37     0.00      19 seastar::reactor::del_timer(seastar::timer<std::chrono::_V2::steady_clock>*)
 38     0.00      19 cql::Connection::sendMessage(cql::Object<cql::RequestMessageBase>&&, cql::ConnectionStream const&)
 39     0.00      19 seastar::reactor::syscall_pollfn::poll()
 40     0.00      19 std::_Function_handler<void (), seastar::reactor::run()::{lambda()#5}>::_M_invoke(std::_Any_data const&)
 41     0.00      17 seastar::reactor::aio_batch_submit_pollfn::poll()
 42     0.00      16 __tls_init
 43     0.00      16 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_mutate(unsigned long, unsigned long, char const*, unsigned long)
 44     0.00      16 seastar::reactor::lowres_timer_pollfn::exit_interrupt_mode()
 45     0.00      16 seastar::reactor::batch_flush_pollfn::poll()
 46     0.00      15 seastar::try_systemwide_memory_barrier()
 47     0.00      15 timer_settime
 48     0.00      13 seastar::reactor::epoll_pollfn::poll()
 49     0.00      13 seastar::thread::try_run_one_yielded_thread()
 50     0.00      13 seastar::reactor::epoll_pollfn::exit_interrupt_mode()
 51     0.00      12 seastar::memory::cpu_pages::allocate_small(unsigned int)
 52     0.00      11 seastar::memory::small_pool::allocate()
 53     0.00      11 seastar::reactor::drain_cross_cpu_freelist_pollfn::poll()
 54     0.00      10 seastar::reactor::lowres_timer_pollfn::poll()
 55     0.00       9 seastar::reactor::add_task(std::unique_ptr<seastar::task, std::default_delete<seastar::task> >&&)
 56     0.00       9 _ZN7seastar8futurizeINS_6futureIJNS_16temporary_bufferIcEEEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS7_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS8_EENS7_6ObjectINS7_19ResponseMessageBaseEEEEEDaSD_SF_ENKUlvE_clEvEUlOS3_E_JS3_EEES4_OSC_OSt5tupleIJDpT0_EE
 57     0.00       9 LZ4_decompress_safe
 58     0.00       8 _ZN7seastar8internal8repeaterIZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSD_3_V212system_clockENSD_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS4_S6_S8_ENKUlvE2_clEvEUlvE_E15run_and_disposeEv
 59     0.00       8 void seastar::promise<>::set_value<>()
 60     0.00       8 seastar::memory::free(void*, unsigned long)
 61     0.00       8 _ZN7seastar8futurizeINS_10bool_classINS_18stop_iteration_tagEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_ENKUlvE_clEvEUlONS_16temporary_bufferIcEEE0_JSO_EEENS_6futureIJS3_EEEOSB_OSt5tupleIJDpT0_EE
 62     0.00       8 cql::BatchCommand::BatchCommand()
 63     0.00       7 seastar::output_stream<char>::write(char const*, unsigned long)
 64     0.00       7 seastar::lowres_clock_impl::update()
 65     0.00       7 cql::Connection::waitNextMessage(cql::ConnectionStream const&)
 66     0.00       7 cql::Session::batchExecute(cql::BatchCommand&&)
 67     0.00       7 seastar::free_deleter_impl::~free_deleter_impl()
 68     0.00       7 seastar::memory::cpu_pages::drain_cross_cpu_freelist()
 69     0.00       6 std::_Hash_bytes(void const*, unsigned long, unsigned long)
 70     0.00       6 seastar::memory::allocate(unsigned long)
 71     0.00       6 read
 72     0.00       6 seastar::reactor::io_pollfn::exit_interrupt_mode()
 73     0.00       6 __send
 74     0.00       6 operator delete(void*, unsigned long)
 75     0.00       6 _ZN7seastar12continuationIZNS_6futureIJEE4thenIZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSF_3_V212system_clockENSF_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS6_S8_SA_ENKUlvE2_clEvENKUlvE_clEvEUlvE_NS1_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEES7_OS5_EUlSV_E_JEE15run_and_disposeEv
 76     0.00       6 cql::Connection::decodeMessage(cql::Object<cql::ResponseMessageBase>&, seastar::temporary_buffer<char>&&) const
 77     0.00       6 seastar::future<unsigned long>::forward_to(seastar::promise<unsigned long>&&)
 78     0.00       6 cql::BatchCommand::getParameterCountOfLastSet() &
 79     0.00       5 seastar::reactor::io_pollfn::try_enter_interrupt_mode()
 80     0.00       5 _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlvE0_JEEES2_OS9_OSt5tupleIJDpT0_EE.isra.704
 81     0.00       5 seastar::reactor::syscall_pollfn::exit_interrupt_mode()
 82     0.00       5 _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedIZNS5_16handle_exceptionIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlNSt15__exception_ptr13exception_ptrEE5_EES5_OSC_EUlS19_E_S5_EESE_S19_EUlS19_E_JS4_EE15run_and_disposeEv
 83     0.00       5 _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedINS5_12finally_bodyIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlvE6_Lb0EEES5_EESE_OSC_EUlS18_E_JS4_EE15run_and_disposeEv
 84     0.00       5 seastar::future<seastar::temporary_buffer<char> > seastar::futurize<seastar::future<seastar::temporary_buffer<char> > >::apply<seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}, seastar::temporary_buffer<char> >(seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}&&, std::tuple<seastar::temporary_buffer<char> >&&)
 85     0.00       5 cql::Object<cql::BatchCommandData>::Object(std::unique_ptr<cql::BatchCommandData, std::default_delete<cql::BatchCommandData> >&&)::{lambda(void*)#1}::_FUN(void*)
 86     0.00       5 _ZN7seastar12continuationIZNS_6futureIJN3cql6ObjectINS2_19ResponseMessageBaseEEEEE4thenIZZZNS2_7Session12batchExecuteEONS2_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS9_NS_13lw_shared_ptrINS2_14ConnectionPoolEEENS2_12_GLOBAL__N_19RetryFlowENSR_INS2_10ConnectionEEENS2_16ConnectionStreamENS3_INS2_12BatchMessageEEENS3_INSU_14PrepareResultsEEEEEDaSC_SE_SG_SI_SK_SM_SO_ENKUlvE_clEvEUlSB_E3_NS1_IJEEEEESD_OSB_EUlS16_E_JS5_EE15run_and_disposeEv
 87     0.00       5 seastar::reactor::signal_pollfn::poll()
 88     0.00       5 seastar::reactor::smp_pollfn::poll()
 89     0.00       5 seastar::reactor::signal_pollfn::try_enter_interrupt_mode()
 90     0.00       4 seastar::promise<>::promise(seastar::promise<>&&)
 91     0.00       4 seastar::reactor::add_timer(seastar::timer<std::chrono::_V2::steady_clock>*)
 92     0.00       4 _ZZZN3cql10Connection15waitNextMessageERKNS_16ConnectionStreamEENKUlRT_RT0_E_clIN7seastar13lw_shared_ptrIS0_EENS_6ObjectINS_19ResponseMessageBaseEEEEEDaS5_S7_ENKUlvE_clEv
 93     0.00       4 seastar::reactor::write_all_part(seastar::pollable_fd_state&, void const*, unsigned long, unsigned long)
 94     0.00       4 seastar::reactor::syscall_pollfn::try_enter_interrupt_mode()
 95     0.00       4 sigfillset
 96     0.00       4 LZ4_compress_default
 97     0.00       4 seastar::reactor::add_urgent_task(std::unique_ptr<seastar::task, std::default_delete<seastar::task> >&&)
 98     0.00       4 seastar::reactor::execution_stage_pollfn::try_enter_interrupt_mode()
 99     0.00       4 _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlS9_E3_JNSX_INS5_19ResponseMessageBaseEEEEEES2_OS9_OSt5tupleIJDpT0_EE
```

### CpuSampleHotPathAnalyzer

``` text
collect for benchmark in 180000 ms
- SymbolName Samples (Overhead)
- root 6408 (1.00)
  - epoll_pwait 1470 (0.23)
  - brnf_frag_data_storage 1151 (0.18)
    - seastar::reactor_backend_epoll::~reactor_backend_epoll() 814 (0.13)
      - seastar::internal::io_getevents(unsigned long, long, long, io_event*, timespec const*) 122 (0.02)
      - pthread_sigmask 85 (0.01)
      - seastar::reactor::sleep() 69 (0.01)
        - __clock_gettime 4 (0.00)
      - seastar::reactor::process_io() 68 (0.01)
      - seastar::reactor_backend_epoll::wait_and_process(int, __sigset_t const*) 51 (0.01)
      - seastar::reactor::epoll_pollfn::exit_interrupt_mode() 37 (0.01)
      - epoll_pwait 36 (0.01)
      - seastar::reactor::flush_pending_aio() 31 (0.00)
      - __errno_location 28 (0.00)
      - seastar::reactor::lowres_timer_pollfn::exit_interrupt_mode() 12 (0.00)
      - seastar::basic_semaphore<seastar::semaphore_default_exception_factory, std::chrono::_V2::steady_clock>::signal(unsigned long) [clone .part.3035] 11 (0.00)
      - seastar::continuation<seastar::future<seastar::temporary_buffer<char> > seastar::future<seastar::temporary_buffer<char> >::then<seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}, seastar::future<seastar::temporary_buffer<char> > >(seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}&&)::{lambda(auto:2)#1}, seastar::temporary_buffer<char> >::run_and_dispose() 11 (0.00)
      - void seastar::promise<>::set_value<>() 9 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedIZNS5_16handle_exceptionIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlNSt15__exception_ptr13exception_ptrEE5_EES5_OSC_EUlS19_E_S5_EESE_S19_EUlS19_E_JS4_EE15run_and_disposeEv 9 (0.00)
      - seastar::reactor::io_pollfn::exit_interrupt_mode() 8 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJEE4thenIZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSF_3_V212system_clockENSF_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS6_S8_SA_ENKUlvE2_clEvENKUlvE_clEvEUlvE_NS1_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEES7_OS5_EUlSV_E_JEE15run_and_disposeEv 7 (0.00)
      - seastar::reactor::io_pollfn::try_enter_interrupt_mode() 6 (0.00)
      - seastar::continuation<seastar::future<unsigned long> seastar::future<>::then<seastar::reactor::read_some(seastar::pollable_fd_state&, void*, unsigned long)::{lambda()#1}, seastar::future<unsigned long> >(seastar::reactor::read_some(seastar::pollable_fd_state&, void*, unsigned long)::{lambda()#1}&&)::{lambda(auto:1)#1}>::run_and_dispose() 6 (0.00)
      - _ZN7seastar8internal8repeaterIZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSD_3_V212system_clockENSD_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS4_S6_S8_ENKUlvE2_clEvEUlvE_E15run_and_disposeEv 5 (0.00)
      - seastar::reactor::del_timer(seastar::timer<std::chrono::_V2::steady_clock>*) 5 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJNS_16temporary_bufferIcEEEE4thenIZZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_ENKUlvE_clEvEUlOS3_E0_NS1_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEESD_OSB_EUlST_E_JS3_EE15run_and_disposeEv 4 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJN3cql6ObjectINS2_19ResponseMessageBaseEEEEE4thenIZZZNS2_7Session12batchExecuteEONS2_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS9_NS_13lw_shared_ptrINS2_14ConnectionPoolEEENS2_12_GLOBAL__N_19RetryFlowENSR_INS2_10ConnectionEEENS2_16ConnectionStreamENS3_INS2_12BatchMessageEEENS3_INSU_14PrepareResultsEEEEEDaSC_SE_SG_SI_SK_SM_SO_ENKUlvE_clEvEUlSB_E3_NS1_IJEEEEESD_OSB_EUlS16_E_JS5_EE15run_and_disposeEv 4 (0.00)
      - seastar::reactor::syscall_pollfn::try_enter_interrupt_mode() 4 (0.00)
      - seastar::reactor::aio_batch_submit_pollfn::try_enter_interrupt_mode() 3 (0.00)
      - seastar::reactor::smp_pollfn::try_enter_interrupt_mode() 3 (0.00)
      - _ZN7seastar8internal8repeaterIZZN3cql7Session12batchExecuteEONS2_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS4_NS_13lw_shared_ptrINS2_14ConnectionPoolEEENS2_12_GLOBAL__N_19RetryFlowENSM_INS2_10ConnectionEEENS2_16ConnectionStreamENS2_6ObjectINS2_12BatchMessageEEENSU_INSP_14PrepareResultsEEEEEDaS7_S9_SB_SD_SF_SH_SJ_EUlvE_E15run_and_disposeEv 3 (0.00)
      - _ZNKSt14default_deleteIN7seastar8internal8repeaterIZZN3cql7Session12batchExecuteEONS3_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS5_NS0_13lw_shared_ptrINS3_14ConnectionPoolEEENS3_12_GLOBAL__N_19RetryFlowENSN_INS3_10ConnectionEEENS3_16ConnectionStreamENS3_6ObjectINS3_12BatchMessageEEENSV_INSQ_14PrepareResultsEEEEEDaS8_SA_SC_SE_SG_SI_SK_EUlvE_EEEclEPS11_.isra.434 3 (0.00)
      - seastar::internal::do_with_state<std::tuple<seastar::lw_shared_ptr<cql::Connection>, cql::Object<cql::ResponseMessageBase> >, seastar::future<> >::run_and_dispose() 3 (0.00)
      - seastar::reactor::signal_pollfn::try_enter_interrupt_mode() 3 (0.00)
      - seastar::continuation<seastar::future<seastar::temporary_buffer<char> > seastar::future<unsigned long>::then<seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}, seastar::future<seastar::temporary_buffer<char> > >(seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}&&)::{lambda(auto:1)#1}, unsigned long>::run_and_dispose() 3 (0.00)
      - _ZN7seastar8futurizeINS_10bool_classINS_18stop_iteration_tagEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_ENKUlvE_clEvEUlONS_16temporary_bufferIcEEE0_JSO_EEENS_6futureIJS3_EEEOSB_OSt5tupleIJDpT0_EE 3 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJNS_16temporary_bufferIcEEEE4thenIZZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_ENKUlvE_clEvEUlOS3_E_S4_EESD_OSB_EUlSP_E_JS3_EE15run_and_disposeEv 3 (0.00)
      - seastar::reactor::drain_cross_cpu_freelist_pollfn::exit_interrupt_mode() 2 (0.00)
      - seastar::reactor::insert_active_task_queue(seastar::reactor::task_queue*) 2 (0.00)
      - seastar::reactor::lowres_timer_pollfn::try_enter_interrupt_mode() 2 (0.00)
      - seastar::reactor::signal_pollfn::exit_interrupt_mode() 2 (0.00)
      - brnf_frag_data_storage 2 (0.00)
        - seastar::reactor::sleep() 1 (0.00)
          - __clock_gettime 1 (0.00)
        - cql::MessageHeader::getOpCode() const 1 (0.00)
      - _ZN7seastar8futurizeINS_6futureIJNS_16temporary_bufferIcEEEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS7_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS8_EENS7_6ObjectINS7_19ResponseMessageBaseEEEEEDaSD_SF_ENKUlvE_clEvEUlOS3_E_JS3_EEES4_OSC_OSt5tupleIJDpT0_EE 2 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJEE12then_wrappedIZNS2_16handle_exceptionIZZN3cql10Connection15waitNextMessageERKNS5_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS6_EENS5_6ObjectINS5_19ResponseMessageBaseEEEEEDaSB_SD_EUlNSt15__exception_ptr13exception_ptrEE0_EES2_OSA_EUlSO_E_S2_EESC_SO_EUlSO_E_JEE15run_and_disposeEv 2 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJEE4thenIZZZN3cql7Session12batchExecuteEONS4_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS6_NS_13lw_shared_ptrINS4_14ConnectionPoolEEENS4_12_GLOBAL__N_19RetryFlowENSO_INS4_10ConnectionEEENS4_16ConnectionStreamENS4_6ObjectINS4_12BatchMessageEEENSW_INSR_14PrepareResultsEEEEEDaS9_SB_SD_SF_SH_SJ_SL_ENKUlvE_clEvEUlvE4_NS1_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEESA_OS8_EUlS17_E_JEE15run_and_disposeEv 2 (0.00)
      - seastar::internal::do_with_state<std::tuple<cql::BatchCommand, seastar::lw_shared_ptr<cql::ConnectionPool>, cql::(anonymous namespace)::RetryFlow, seastar::lw_shared_ptr<cql::Connection>, cql::ConnectionStream, cql::Object<cql::BatchMessage>, cql::Object<cql::(anonymous namespace)::PrepareResults> >, seastar::future<> >::run_and_dispose() 2 (0.00)
      - cql::ConnectionStream::isValid() const 2 (0.00)
      - __clock_gettime 2 (0.00)
      - seastar::reactor::execution_stage_pollfn::try_enter_interrupt_mode() 1 (0.00)
      - seastar::reactor::aio_batch_submit_pollfn::exit_interrupt_mode() 1 (0.00)
      - seastar::reactor::insert_activating_task_queues() 1 (0.00)
      - cql::ConnectionPool::returnConnection(seastar::lw_shared_ptr<cql::Connection>&&, cql::ConnectionStream&&) 1 (0.00)
      - seastar::reactor::epoll_pollfn::try_enter_interrupt_mode() 1 (0.00)
      - seastar::reactor::run_tasks(seastar::reactor::task_queue&) 1 (0.00)
      - _ZNKSt14default_deleteIN7seastar8internal8repeaterIZZN3cql10Connection15waitNextMessageERKNS3_16ConnectionStreamEENKUlRT_RT0_E_clINS0_13lw_shared_ptrIS4_EENS3_6ObjectINS3_19ResponseMessageBaseEEEEEDaS9_SB_EUlvE_EEEclEPSK_.isra.303 1 (0.00)
      - seastar::promise<>::abandoned() [clone .isra.340] 1 (0.00)
      - _ZN7seastar8internal8repeaterIZZN3cql10Connection15waitNextMessageERKNS2_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS3_EENS2_6ObjectINS2_19ResponseMessageBaseEEEEEDaS8_SA_EUlvE_E15run_and_disposeEv 1 (0.00)
      - seastar::reactor::smp_pollfn::exit_interrupt_mode() 1 (0.00)
      - seastar::continuation_base<seastar::bool_class<seastar::stop_iteration_tag> >::~continuation_base() 1 (0.00)
      - _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedINS5_12finally_bodyIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlvE6_Lb0EEES5_EESE_OSC_EUlS18_E_JS4_EE15run_and_disposeEv 1 (0.00)
      - seastar::reactor::execution_stage_pollfn::exit_interrupt_mode() 1 (0.00)
    - seastar::reactor::smp_pollfn::~smp_pollfn() 118 (0.02)
      - __mprotect 33 (0.01)
      - __getpagesize 31 (0.00)
      - seastar::systemwide_memory_barrier() 25 (0.00)
      - seastar::try_systemwide_memory_barrier() 21 (0.00)
      - seastar::smp::poll_queues() 5 (0.00)
      - seastar::reactor::smp_pollfn::try_enter_interrupt_mode() 1 (0.00)
    - __clock_gettime 48 (0.01)
    - cql::Connection::encodeMessage(cql::Object<cql::RequestMessageBase> const&) 39 (0.01)
      - cql::BatchMessage::encodeBody(cql::ConnectionInfo const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) const 25 (0.00)
      - cql::BatchCommand::isValid() const 2 (0.00)
      - cql::BatchCommand::getQueries() const & 1 (0.00)
      - cql::ProtocolBatchParameters::encode(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) const 1 (0.00)
    - seastar::reactor::drain_cross_cpu_freelist_pollfn::~drain_cross_cpu_freelist_pollfn() 21 (0.00)
      - seastar::reactor::sleep() 19 (0.00)
        - __clock_gettime 14 (0.00)
      - brnf_frag_data_storage 2 (0.00)
        - seastar::reactor::sleep() 2 (0.00)
    - brnf_frag_data_storage 13 (0.00)
      - brnf_frag_data_storage 10 (0.00)
        - brnf_frag_data_storage 4 (0.00)
          - brnf_frag_data_storage 3 (0.00)
            - brnf_frag_data_storage 3 (0.00)
              - brnf_frag_data_storage 3 (0.00)
                - brnf_frag_data_storage 3 (0.00)
                  - brnf_frag_data_storage 3 (0.00)
                    - brnf_frag_data_storage 3 (0.00)
                      - brnf_frag_data_storage 3 (0.00)
                        - brnf_frag_data_storage 3 (0.00)
                          - brnf_frag_data_storage 3 (0.00)
                            - brnf_frag_data_storage 3 (0.00)
                              - brnf_frag_data_storage 3 (0.00)
                                - brnf_frag_data_storage 3 (0.00)
                                  - brnf_frag_data_storage 3 (0.00)
                                    - brnf_frag_data_storage 3 (0.00)
                                      - brnf_frag_data_storage 3 (0.00)
                                        - brnf_frag_data_storage 3 (0.00)
                                          - brnf_frag_data_storage 3 (0.00)
                                            - brnf_frag_data_storage 3 (0.00)
                                              - brnf_frag_data_storage 3 (0.00)
                                                - brnf_frag_data_storage 3 (0.00)
                                                  - brnf_frag_data_storage 3 (0.00)
                                                    - brnf_frag_data_storage 3 (0.00)
                                                      - brnf_frag_data_storage 3 (0.00)
                                                        - brnf_frag_data_storage 3 (0.00)
                                                          - brnf_frag_data_storage 3 (0.00)
                                                            - brnf_frag_data_storage 3 (0.00)
                                                              - brnf_frag_data_storage 3 (0.00)
                                                                - brnf_frag_data_storage 3 (0.00)
                                                                  - brnf_frag_data_storage 3 (0.00)
                                                                    - brnf_frag_data_storage 3 (0.00)
                                                                      - brnf_frag_data_storage 3 (0.00)
                                                                        - brnf_frag_data_storage 3 (0.00)
                                                                          - brnf_frag_data_storage 3 (0.00)
                                                                            - brnf_frag_data_storage 3 (0.00)
                                                                              - brnf_frag_data_storage 3 (0.00)
                                                                                - brnf_frag_data_storage 3 (0.00)
                                                                                  - brnf_frag_data_storage 3 (0.00)
                                                                                    - brnf_frag_data_storage 3 (0.00)
                                                                                      - brnf_frag_data_storage 3 (0.00)
                                                                                        - brnf_frag_data_storage 3 (0.00)
                                                                                          - brnf_frag_data_storage 3 (0.00)
                                                                                            - brnf_frag_data_storage 3 (0.00)
                                                                                              - brnf_frag_data_storage 3 (0.00)
                                                                                                - brnf_frag_data_storage 3 (0.00)
                                                                                                  - brnf_frag_data_storage 3 (0.00)
                                                                                                    - brnf_frag_data_storage 3 (0.00)
                                                                                                      - brnf_frag_data_storage 3 (0.00)
                                                                                                        - brnf_frag_data_storage 3 (0.00)
                                                                                                          - brnf_frag_data_storage 3 (0.00)
                                                                                                            - brnf_frag_data_storage 3 (0.00)
                                                                                                              - brnf_frag_data_storage 3 (0.00)
                                                                                                                - brnf_frag_data_storage 3 (0.00)
                                                                                                                  - brnf_frag_data_storage 3 (0.00)
                                                                                                                    - brnf_frag_data_storage 3 (0.00)
                                                                                                                      - brnf_frag_data_storage 3 (0.00)
                                                                                                                        - brnf_frag_data_storage 3 (0.00)
                                                                                                                          - brnf_frag_data_storage 3 (0.00)
                                                                                                                            - brnf_frag_data_storage 3 (0.00)
                                                                                                                              - brnf_frag_data_storage 3 (0.00)
                                                                                                                                - _ZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSA_3_V212system_clockENSA_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS1_S3_S5_ENKUlvE2_clEvENKUlvE_clEv 2 (0.00)
                                                                                                                                - seastar::promise<seastar::temporary_buffer<char> >::promise(seastar::promise<seastar::temporary_buffer<char> >&&) 1 (0.00)
          - seastar::basic_sstring<char, unsigned int, 15u> boost::algorithm::replace_all_copy<seastar::basic_sstring<char, unsigned int, 15u>, char [2], char [2]>(seastar::basic_sstring<char, unsigned int, 15u> const&, char const (&) [2], char const (&) [2]) 1 (0.00)
        - seastar::future<unsigned long> seastar::future<>::then<seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long)::{lambda()#1}, seastar::future<unsigned long> >(seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long)::{lambda()#1}&&) 3 (0.00)
        - __send 2 (0.00)
        - __pthread_enable_asynccancel 1 (0.00)
      - std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) 3 (0.00)
        - seastar::future<unsigned long> seastar::futurize<seastar::future<unsigned long> >::apply<seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long)::{lambda()#1}>(seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long)::{lambda()#1}&&, std::tuple<>&&) [clone .isra.566] 1 (0.00)
        - __pthread_enable_asynccancel 1 (0.00)
        - __send 1 (0.00)
    - _ZN7seastar8internal8repeaterIZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSD_3_V212system_clockENSD_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS4_S6_S8_ENKUlvE2_clEvEUlvE_ED0Ev 8 (0.00)
      - cql::BatchCommand::BatchCommand() 5 (0.00)
      - _ZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSA_3_V212system_clockENSA_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS1_S3_S5_ENKUlvE2_clEvENKUlvE_clEv 3 (0.00)
    - seastar::memory::cpu_pages::free(void*) 5 (0.00)
    - cql::Connection::waitNextMessage(cql::ConnectionStream const&) 5 (0.00)
    - read 4 (0.00)
    - seastar::future<>::operator=(seastar::future<>&&) 4 (0.00)
    - std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::resize(unsigned long, char) 4 (0.00)
    - seastar::future<> seastar::future<>::then<seastar::output_stream<char>::poll_flush()::{lambda()#1}, seastar::future<> >(seastar::output_stream<char>::poll_flush()::{lambda()#1}&&) 3 (0.00)
    - seastar::future<seastar::bool_class<seastar::stop_iteration_tag> >::forward_to(seastar::promise<seastar::bool_class<seastar::stop_iteration_tag> >&&) 3 (0.00)
    - void seastar::promise<>::set_value<>() 3 (0.00)
    - _ZZZN3cql10Connection15waitNextMessageERKNS_16ConnectionStreamEENKUlRT_RT0_E_clIN7seastar13lw_shared_ptrIS0_EENS_6ObjectINS_19ResponseMessageBaseEEEEEDaS5_S7_ENKUlvE_clEv 3 (0.00)
    - cql::BatchMessage::encodeBody(cql::ConnectionInfo const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) const 2 (0.00)
    - seastar::net::posix_data_source_impl::~posix_data_source_impl() 2 (0.00)
      - seastar::continuation<seastar::future<seastar::temporary_buffer<char> > seastar::future<unsigned long>::then<seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}, seastar::future<seastar::temporary_buffer<char> > >(seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}&&)::{lambda(auto:1)#1}, unsigned long>::run_and_dispose() 2 (0.00)
    - seastar::internal::do_with_state<std::tuple<seastar::lw_shared_ptr<cql::Connection>, cql::Object<cql::ResponseMessageBase> >, seastar::future<> >::~do_with_state() 2 (0.00)
      - seastar::future<>::set_callback(std::unique_ptr<seastar::continuation_base<>, std::default_delete<seastar::continuation_base<> > >) 2 (0.00)
    - _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlS9_E3_JNSX_INS5_19ResponseMessageBaseEEEEEES2_OS9_OSt5tupleIJDpT0_EE 2 (0.00)
    - _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZNS2_16handle_exceptionIZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_EUlNSt15__exception_ptr13exception_ptrEE0_EES2_OSB_EUlSP_E_JS2_EEES2_SP_DpOT0_ 2 (0.00)
    - std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) 2 (0.00)
    - seastar::future<> seastar::future<>::then_wrapped<seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2}, seastar::future<> >(seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2}&&) 2 (0.00)
    - seastar::continuation<seastar::future<seastar::connected_socket> seastar::future<>::then<seastar::net::posix_socket_impl::connect(seastar::socket_address, seastar::socket_address, seastar::transport)::{lambda()#1}, seastar::future<seastar::connected_socket> >(seastar::net::posix_socket_impl::connect(seastar::socket_address, seastar::socket_address, seastar::transport)::{lambda()#1}&&)::{lambda(auto:1)#1}>::run_and_dispose() 1 (0.00)
    - cql::LZ4Compressor::compress(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) 1 (0.00)
    - seastar::net::posix_data_sink_impl::~posix_data_sink_impl() 1 (0.00)
      - seastar::add_to_flush_poller(seastar::output_stream<char>*) 1 (0.00)
    - seastar::memory::free(void*) 1 (0.00)
    - cql::BatchMessage::~BatchMessage() 1 (0.00)
      - brnf_frag_data_storage 1 (0.00)
        - seastar::memory::cpu_pages::free(void*) 1 (0.00)
    - seastar::reactor::add_task(std::unique_ptr<seastar::task, std::default_delete<seastar::task> >&&) 1 (0.00)
    - seastar::memory::cpu_pages::free(void*, unsigned long) 1 (0.00)
    - std::pair<seastar::lw_shared_ptr<cql::Connection>, cql::ConnectionStream>::~pair() 1 (0.00)
    - _ZZN7seastar6futureIJNS_16temporary_bufferIcEEEE4thenIZZZN3cql10Connection15waitNextMessageERKNS5_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS6_EENS5_6ObjectINS5_19ResponseMessageBaseEEEEEDaSB_SD_ENKUlvE_clEvEUlOS2_E0_NS0_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEESC_OSA_ENUlSS_E_D1Ev 1 (0.00)
    - LZ4_compress_fast_extState 1 (0.00)
    - seastar::input_stream<char>::read_exactly(unsigned long) 1 (0.00)
    - seastar::output_stream<char>::poll_flush() 1 (0.00)
    - seastar::promise<cql::Object<cql::ResponseMessageBase> >::abandoned() [clone .isra.284] 1 (0.00)
    - seastar::future<seastar::temporary_buffer<char> > seastar::future<seastar::temporary_buffer<char> >::then<seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}, seastar::future<seastar::temporary_buffer<char> > >(seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}&&) 1 (0.00)
    - seastar::promise<>::promise(seastar::promise<>&&) 1 (0.00)
    - cql::MessageHeader::MessageHeader() 1 (0.00)
    - __pthread_disable_asynccancel 1 (0.00)
    - seastar::deleter::~deleter() 1 (0.00)
    - seastar::future<seastar::temporary_buffer<char> > seastar::future<seastar::temporary_buffer<char> >::then<seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}, seastar::future<seastar::temporary_buffer<char> > >(seastar::input_stream<char>::read_exactly(unsigned long)::{lambda(auto:1)#1}&&)::{lambda(auto:2)#1}::~then() 1 (0.00)
    - LZ4_compressBound 1 (0.00)
    - seastar::promise<seastar::bool_class<seastar::stop_iteration_tag> >::promise(seastar::promise<seastar::bool_class<seastar::stop_iteration_tag> >&&) 1 (0.00)
    - boost::program_options::variables_map::~variables_map() 1 (0.00)
      - cql::BatchCommand::openParameterSet() & 1 (0.00)
    - munmap 1 (0.00)
    - boost::detail::sp_counted_impl_p<boost::program_options::value_semantic const>::~sp_counted_impl_p() 1 (0.00)
      - brnf_frag_data_storage 1 (0.00)
        - boost::detail::sp_counted_impl_p<boost::program_options::value_semantic const>::dispose() 1 (0.00)
    - seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long) 1 (0.00)
    - __pthread_enable_asynccancel 1 (0.00)
    - seastar::promise<seastar::temporary_buffer<char> >::abandoned() [clone .isra.299] 1 (0.00)
    - seastar::net::posix_data_sink_impl::put(seastar::temporary_buffer<char>) 1 (0.00)
  - __clock_gettime 478 (0.07)
    - brnf_frag_data_storage 3 (0.00)
      - void seastar::promise<>::set_value<>() 2 (0.00)
      - seastar::reactor::activate(seastar::reactor::task_queue&) [clone .part.2741] 1 (0.00)
  - seastar::smp::poll_queues() 404 (0.06)
  - std::_Function_handler<bool (), seastar::reactor::run()::{lambda()#7}>::_M_invoke(std::_Any_data const&) 339 (0.05)
  - seastar::reactor::run() 242 (0.04)
  - seastar::smp_message_queue::flush_response_batch() 211 (0.03)
  - timerfd_settime 162 (0.03)
  - unsigned long seastar::smp_message_queue::process_queue<4ul, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}>(seastar::smp_message_queue::lf_queue&, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}) [clone .isra.3534] 114 (0.02)
  - unsigned long seastar::smp_message_queue::process_queue<2ul, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}>(seastar::smp_message_queue::lf_queue&, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}) [clone .isra.3535] 111 (0.02)
  - seastar::reactor::io_pollfn::poll() 91 (0.01)
  - seastar::reactor::signals::action(int, siginfo_t*, void*) 86 (0.01)
  - pthread_sigmask 74 (0.01)
  - cql::Object<cql::BatchCommandData>::Object(std::unique_ptr<cql::BatchCommandData, std::default_delete<cql::BatchCommandData> >&&)::{lambda(void*)#1}::_FUN(void*) 69 (0.01)
    - _ZZZN3cql7Session12batchExecuteEONS_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS1_N7seastar13lw_shared_ptrINS_14ConnectionPoolEEENS_12_GLOBAL__N_19RetryFlowENSK_INS_10ConnectionEEENS_16ConnectionStreamENS_6ObjectINS_12BatchMessageEEENSS_INSN_14PrepareResultsEEEEEDaS4_S6_S8_SA_SC_SE_SG_ENKUlvE_clEv 30 (0.00)
      - _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlvE0_JEEES2_OS9_OSt5tupleIJDpT0_EE.isra.704 9 (0.00)
        - cql::(anonymous namespace)::prepareQueries(cql::BatchCommand&, cql::(anonymous namespace)::RetryFlow&, seastar::lw_shared_ptr<cql::Connection>&, cql::ConnectionStream&, cql::Object<cql::BatchMessage>&, cql::Object<cql::(anonymous namespace)::PrepareResults>&) 6 (0.00)
        - cql::NodeConfiguration::getPreparedQueryId[abi:cxx11](cql::StringHolder const&) & 2 (0.00)
        - cql::SessionConfiguration::getLogger() const & 1 (0.00)
      - cql::ProtocolBatchParameters::setBatchCommandRef(cql::BatchCommand&) 2 (0.00)
      - cql::(anonymous namespace)::prepareQueries(cql::BatchCommand&, cql::(anonymous namespace)::RetryFlow&, seastar::lw_shared_ptr<cql::Connection>&, cql::ConnectionStream&, cql::Object<cql::BatchMessage>&, cql::Object<cql::(anonymous namespace)::PrepareResults>&) 1 (0.00)
      - cql::Object<cql::BatchMessage> cql::RequestMessageFactory::makeRequestMessage<cql::BatchMessage>() 1 (0.00)
    - seastar::add_to_flush_poller(seastar::output_stream<char>*) 6 (0.00)
    - cql::Connection::sendMessage(cql::Object<cql::RequestMessageBase>&&, cql::ConnectionStream const&) 4 (0.00)
    - _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlvE0_JEEES2_OS9_OSt5tupleIJDpT0_EE.isra.704 3 (0.00)
    - cql::Connection::waitNextMessage(cql::ConnectionStream const&) 3 (0.00)
    - seastar::promise<seastar::bool_class<seastar::stop_iteration_tag> >::promise(seastar::promise<seastar::bool_class<seastar::stop_iteration_tag> >&&) 3 (0.00)
    - std::_Head_base<0ul, seastar::lw_shared_ptr<cql::Connection>, false>::~_Head_base() 3 (0.00)
    - seastar::output_stream<char>::flush() 2 (0.00)
    - cql::BatchCommand::BatchCommand(decltype(nullptr)) 1 (0.00)
    - cql::Object<cql::ResultMessage>::Object(std::unique_ptr<cql::ResultMessage, std::default_delete<cql::ResultMessage> >&&)::{lambda(void*)#1}::_FUN(void*) 1 (0.00)
    - seastar::promise<seastar::bool_class<seastar::stop_iteration_tag> >::abandoned() [clone .isra.601] 1 (0.00)
    - cql::LZ4Compressor::decompress(cql::ConnectionInfo const&, seastar::temporary_buffer<char>&&) 1 (0.00)
    - cql::BatchCommand::getSerialConsistency() const & 1 (0.00)
    - cql::SocketHolder::out() 1 (0.00)
    - cql::ConnectionStream::ConnectionStream(cql::ConnectionStream&&) 1 (0.00)
    - seastar::output_stream<char>::write(char const*, unsigned long) 1 (0.00)
    - _ZZN7seastar6futureIJEE4thenIZZZN3cql7Session12batchExecuteEONS3_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS5_NS_13lw_shared_ptrINS3_14ConnectionPoolEEENS3_12_GLOBAL__N_19RetryFlowENSN_INS3_10ConnectionEEENS3_16ConnectionStreamENS3_6ObjectINS3_12BatchMessageEEENSV_INSQ_14PrepareResultsEEEEEDaS8_SA_SC_SE_SG_SI_SK_ENKUlvE_clEvEUlvE4_NS0_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEES9_OS7_ENUlS16_E_D2Ev 1 (0.00)
    - seastar::promise<>::abandoned() [clone .isra.340] 1 (0.00)
    - cql::ConnectionPool::getConnection() 1 (0.00)
  - seastar::syscall_work_queue::complete() 62 (0.01)
  - seastar::basic_semaphore<seastar::semaphore_default_exception_factory, std::chrono::_V2::steady_clock>::signal(unsigned long) [clone .part.3035] 60 (0.01)
  - seastar::reactor::execution_stage_pollfn::poll() 59 (0.01)
  - _ZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSA_3_V212system_clockENSA_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS1_S3_S5_ENKUlvE2_clEvENKUlvE_clEv 43 (0.01)
    - cql::Session::batchExecute(cql::BatchCommand&&) 5 (0.00)
    - _ZZZN3cql7Session12batchExecuteEONS_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS1_N7seastar13lw_shared_ptrINS_14ConnectionPoolEEENS_12_GLOBAL__N_19RetryFlowENSK_INS_10ConnectionEEENS_16ConnectionStreamENS_6ObjectINS_12BatchMessageEEENSS_INSN_14PrepareResultsEEEEEDaS4_S6_S8_SA_SC_SE_SG_ENKUlvE_clEv 2 (0.00)
    - std::_Tuple_impl<0ul, cql::BatchCommand, seastar::lw_shared_ptr<cql::ConnectionPool>, cql::(anonymous namespace)::RetryFlow, seastar::lw_shared_ptr<cql::Connection>, cql::ConnectionStream, cql::Object<cql::BatchMessage>, cql::Object<cql::(anonymous namespace)::PrepareResults> >::~_Tuple_impl() 1 (0.00)
    - cql::BatchCommand::getQueries() & 1 (0.00)
    - cql::BatchCommand::isValid() const 1 (0.00)
    - cql::BatchCommand::getConsistency() const & 1 (0.00)
  - std::chrono::_V2::steady_clock::now() 40 (0.01)
  - seastar::reactor::do_expire_lowres_timers() 36 (0.01)
  - seastar::reactor::flush_tcp_batches() 35 (0.01)
    - brnf_frag_data_storage 5 (0.00)
      - seastar::reactor::write_all_part(seastar::pollable_fd_state&, void const*, unsigned long, unsigned long) 3 (0.00)
      - seastar::net::posix_data_sink_impl::put(seastar::temporary_buffer<char>) 2 (0.00)
  - seastar::reactor::signals::poll_signal() 34 (0.01)
  - LZ4_compress_fast_extState 32 (0.00)
  - seastar::reactor::process_io() 32 (0.00)
  - seastar::memory::drain_cross_cpu_freelist() 28 (0.00)
  - std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace_aux(unsigned long, unsigned long, unsigned long, char) 26 (0.00)
  - seastar::reactor_backend_epoll::wait_and_process(int, __sigset_t const*) 25 (0.00)
  - seastar::thread::try_run_one_yielded_thread() 25 (0.00)
  - std::_Function_handler<void (), seastar::reactor::run()::{lambda()#5}>::_M_invoke(std::_Any_data const&) 24 (0.00)
  - cql::BatchCommand::openParameterSet() & 23 (0.00)
  - seastar::internal::io_getevents(unsigned long, long, long, io_event*, timespec const*) 22 (0.00)
  - seastar::memory::cpu_pages::free(void*) 20 (0.00)
  - std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) 20 (0.00)
  - seastar::reactor::flush_pending_aio() 19 (0.00)
  - seastar::memory::small_pool::allocate() 19 (0.00)
  - seastar::memory::cpu_pages::allocate_small(unsigned int) 17 (0.00)
  - __tls_init 15 (0.00)
  - seastar::memory::cpu_pages::drain_cross_cpu_freelist() 14 (0.00)
  - read 14 (0.00)
  - sigfillset 14 (0.00)
  - seastar::reactor::syscall_pollfn::poll() 13 (0.00)
  - seastar::reactor::sleep() 13 (0.00)
    - __clock_gettime 12 (0.00)
  - seastar::reactor::smp_pollfn::poll() 12 (0.00)
  - cql::Connection::sendMessage(cql::Object<cql::RequestMessageBase>&&, cql::ConnectionStream const&) 12 (0.00)
  - timer_settime 12 (0.00)
  - cql::BatchCommand::BatchCommand() 11 (0.00)
  - seastar::reactor::aio_batch_submit_pollfn::poll() 11 (0.00)
  - std::_Hash_bytes(void const*, unsigned long, unsigned long) 10 (0.00)
  - LZ4_decompress_safe 10 (0.00)
  - seastar::reactor::signal_pollfn::poll() 9 (0.00)
  - seastar::reactor::drain_cross_cpu_freelist_pollfn::poll() 8 (0.00)
  - seastar::throw_system_error_on(bool, char const*) 8 (0.00)
  - seastar::reactor::epoll_pollfn::poll() 7 (0.00)
  - seastar::future<> seastar::future<>::then<seastar::output_stream<char>::poll_flush()::{lambda()#1}, seastar::future<> >(seastar::output_stream<char>::poll_flush()::{lambda()#1}&&) 7 (0.00)
  - seastar::reactor::lowres_timer_pollfn::poll() 7 (0.00)
  - seastar::reactor::add_task(std::unique_ptr<seastar::task, std::default_delete<seastar::task> >&&) 7 (0.00)
  - seastar::future<unsigned long>::forward_to(seastar::promise<unsigned long>&&) 7 (0.00)
  - seastar::lowres_clock_impl::update() 6 (0.00)
  - _ZN7seastar8futurizeINS_10bool_classINS_18stop_iteration_tagEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_ENKUlvE_clEvEUlONS_16temporary_bufferIcEEE0_JSO_EEENS_6futureIJS3_EEEOSB_OSt5tupleIJDpT0_EE 6 (0.00)
  - seastar::reactor::enable_timer(std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long, std::ratio<1l, 1000000000l> > >) 6 (0.00)
  - cql::ResultMessage::reset(cql::MessageHeader&&) 5 (0.00)
  - seastar::future<seastar::bool_class<seastar::stop_iteration_tag> >::forward_to(seastar::promise<seastar::bool_class<seastar::stop_iteration_tag> >&&) 5 (0.00)
  - cql::Connection::waitNextMessage(cql::ConnectionStream const&) 5 (0.00)
  - cql::ConnectionStream::close() 5 (0.00)
  - seastar::reactor::batch_flush_pollfn::poll() 5 (0.00)
  - seastar::memory::cpu_pages::free(void*, unsigned long) 5 (0.00)
  - cql::MessageHeader::MessageHeader() 5 (0.00)
  - seastar::free_deleter_impl::~free_deleter_impl() 4 (0.00)
  - seastar::output_stream<char>::write(char const*, unsigned long) 4 (0.00)
  - operator delete(void*, unsigned long) 4 (0.00)
  - seastar::promise<seastar::temporary_buffer<char> >::abandoned() 4 (0.00)
  - _ZN7seastar8futurizeINS_6futureIJNS_16temporary_bufferIcEEEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS7_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS8_EENS7_6ObjectINS7_19ResponseMessageBaseEEEEEDaSD_SF_ENKUlvE_clEvEUlOS3_E_JS3_EEES4_OSC_OSt5tupleIJDpT0_EE 4 (0.00)
  - seastar::data_sink_impl::allocate_buffer(unsigned long) 4 (0.00)
  - seastar::internal::do_with_state<std::tuple<seastar::lw_shared_ptr<cql::Connection>, cql::Object<cql::RequestMessageBase>, seastar::promise<>, seastar::future<> >, seastar::future<> >::~do_with_state() 4 (0.00)
  - seastar::promise<unsigned long>::abandoned() 4 (0.00)
  - seastar::reactor::add_urgent_task(std::unique_ptr<seastar::task, std::default_delete<seastar::task> >&&) 4 (0.00)
  - cql::BatchCommand::getParametersOfLastSet[abi:cxx11]() & 4 (0.00)
  - _ZZZN3cql7Session12batchExecuteEONS_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS1_N7seastar13lw_shared_ptrINS_14ConnectionPoolEEENS_12_GLOBAL__N_19RetryFlowENSK_INS_10ConnectionEEENS_16ConnectionStreamENS_6ObjectINS_12BatchMessageEEENSS_INSN_14PrepareResultsEEEEEDaS4_S6_S8_SA_SC_SE_SG_ENKUlvE_clEv 4 (0.00)
  - cql::Object<cql::(anonymous namespace)::PrepareResults>::Object(std::unique_ptr<cql::(anonymous namespace)::PrepareResults, std::default_delete<cql::(anonymous namespace)::PrepareResults> >&&)::{lambda(void*)#1}::_FUN(void*) 4 (0.00)
  - cql::SessionConfiguration::getMinPoolSize() const 3 (0.00)
  - cql::BatchCommand::getParameterCountOfLastSet() & 3 (0.00)
  - cql::MessageBase::reset(cql::MessageHeader&&) 3 (0.00)
  - __send 3 (0.00)
  - seastar::memory::allocate(unsigned long) 3 (0.00)
    - seastar::reactor_backend_epoll::get_epoll_future(seastar::pollable_fd_state&, seastar::promise<> seastar::pollable_fd_state::*, int) 1 (0.00)
  - seastar::reactor::signal_pollfn::try_enter_interrupt_mode() 3 (0.00)
  - seastar::output_stream<char>::poll_flush() 3 (0.00)
  - seastar::continuation_base<cql::Object<cql::ResponseMessageBase> >::~continuation_base() 3 (0.00)
  - seastar::continuation<seastar::future<unsigned long> seastar::future<>::then<seastar::reactor::read_some(seastar::pollable_fd_state&, void*, unsigned long)::{lambda()#1}, seastar::future<unsigned long> >(seastar::reactor::read_some(seastar::pollable_fd_state&, void*, unsigned long)::{lambda()#1}&&)::{lambda(auto:1)#1}>::run_and_dispose() 3 (0.00)
    - seastar::input_stream<char>::read_exactly(unsigned long) 2 (0.00)
  - seastar::reactor::run_tasks(seastar::reactor::task_queue&) 3 (0.00)
    - __clock_gettime 1 (0.00)
  - std::_Hashtable<cql::StringHolder, std::pair<cql::StringHolder const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<cql::StringHolder const, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >, std::__detail::_Select1st, std::equal_to<cql::StringHolder>, cql::StringHolder::Hash, std::__detail::_Mod_range_hashing, std::__detail::_Default_ranged_hash, std::__detail::_Prime_rehash_policy, std::__detail::_Hashtable_traits<true, false, true> >::_M_find_before_node(unsigned long, cql::StringHolder const&, unsigned long) const 3 (0.00)
  - seastar::future<unsigned long> seastar::futurize<seastar::future<unsigned long> >::apply<seastar::reactor::read_some(seastar::pollable_fd_state&, void*, unsigned long)::{lambda()#1}>(seastar::reactor::read_some(seastar::pollable_fd_state&, void*, unsigned long)::{lambda()#1}&&, std::tuple<>&&) [clone .isra.516] 3 (0.00)
  - operator new(unsigned long) 3 (0.00)
  - seastar::memory::free(void*) 3 (0.00)
  - seastar::future_state<>::forward_to(seastar::promise<>&) 3 (0.00)
  - seastar::future<>::forward_to(seastar::promise<>&&) 3 (0.00)
  - seastar::continuation_base<seastar::temporary_buffer<char> >::~continuation_base() 3 (0.00)
  - cql::SocketHolder::in() 3 (0.00)
  - seastar::future_state<seastar::temporary_buffer<char> >::forward_to(seastar::promise<seastar::temporary_buffer<char> >&) 2 (0.00)
  - std::_Function_handler<seastar::reactor::idle_cpu_handler_result (std::function<bool ()> const&), seastar::reactor::_idle_cpu_handler::{lambda(std::function<bool ()> const&)#1}>::_M_invoke(std::_Any_data const&, std::function<bool ()> const&) 2 (0.00)
  - seastar::memory::small_pool::add_more_objects() 2 (0.00)
  - _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZN3cql10Connection11sendMessageEONS5_6ObjectINS5_18RequestMessageBaseEEERKNS5_16ConnectionStreamEENKUlRT_RT0_RT1_RT2_E_clINS_13lw_shared_ptrIS6_EES9_NS_7promiseIJEEES2_EEDaSF_SH_SJ_SL_EUlvE_JEEES2_OSE_OSt5tupleIJDpT0_EE.isra.614 2 (0.00)
  - openat64 2 (0.00)
  - seastar::internal::do_with_state<std::tuple<cql::BatchCommand, seastar::lw_shared_ptr<cql::ConnectionPool>, cql::(anonymous namespace)::RetryFlow, seastar::lw_shared_ptr<cql::Connection>, cql::ConnectionStream, cql::Object<cql::BatchMessage>, cql::Object<cql::(anonymous namespace)::PrepareResults> >, seastar::future<> >::run_and_dispose() 2 (0.00)
  - cql::Connection::decodeMessage(cql::Object<cql::ResponseMessageBase>&, seastar::temporary_buffer<char>&&) const 2 (0.00)
  - seastar::promise<unsigned long>::get_future() 2 (0.00)
  - seastar::memory::free(void*, unsigned long) 2 (0.00)
    - seastar::reactor::run() 1 (0.00)
  - cql::LZ4Compressor::decompress(cql::ConnectionInfo const&, seastar::temporary_buffer<char>&&) 2 (0.00)
  - seastar::metrics::impl::get_local_impl() 2 (0.00)
  - boost::intrusive::circular_list_algorithms<boost::intrusive::list_node_traits<void*> >::swap_nodes(boost::intrusive::list_node<void*>* const&, boost::intrusive::list_node<void*>* const&) 2 (0.00)
  - seastar::promise<>::abandoned() 2 (0.00)
  - std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::resize(unsigned long, char) 2 (0.00)
  - cql::ResponseMessageFactory::makeResponseMessage(cql::MessageHeader&&) 2 (0.00)
  - _ZZN7seastar6futureIJNS_16temporary_bufferIcEEEE4thenIZZZN3cql10Connection15waitNextMessageERKNS5_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS6_EENS5_6ObjectINS5_19ResponseMessageBaseEEEEEDaSB_SD_ENKUlvE_clEvEUlOS2_E_S3_EESC_OSA_ENUlSO_E_D1Ev 2 (0.00)
  - seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long) 2 (0.00)
  - seastar::net::posix_data_source_impl::get() 2 (0.00)
  - cql::MessageHeader::decodeHeader(cql::ConnectionInfo const&, seastar::temporary_buffer<char>&&) 2 (0.00)
  - std::chrono::_V2::system_clock::now() 2 (0.00)
  - seastar::deleter::~deleter() 2 (0.00)
  - cql::ProtocolResultRowsMetadata::reset() 2 (0.00)
  - seastar::promise<>::promise(seastar::promise<>&&) 2 (0.00)
  - cql::ConnectionStream::~ConnectionStream() 2 (0.00)
  - __libc_free 2 (0.00)
  - cql::NodeConfiguration::getPreparedQueryId[abi:cxx11](cql::StringHolder const&) & 2 (0.00)
  - operator delete(void*) 2 (0.00)
  - cql::ProtocolResultPreparedMetadata::reset() 2 (0.00)
  - seastar::future_state<>::operator=(seastar::future_state<>&&) 2 (0.00)
  - void seastar::promise<>::set_value<>() 1 (0.00)
  - cql::ResultSet::ResultSet(decltype(nullptr)) 1 (0.00)
  - seastar::reactor::run_some_tasks(std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long, std::ratio<1l, 1000000000l> > >&) [clone .part.2743] 1 (0.00)
  - cql::MessageHeader::setStreamId(unsigned long) 1 (0.00)
  - cql::ConnectionPool::tryGetConnection() 1 (0.00)
  - seastar::basic_sstring<char, unsigned int, 15u>::basic_sstring(seastar::basic_sstring<char, unsigned int, 15u> const&) 1 (0.00)
  - seastar::reactor::add_timer(seastar::timer<std::chrono::_V2::steady_clock>*) 1 (0.00)
  - seastar::future<unsigned long> seastar::future<>::then<seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long)::{lambda()#1}, seastar::future<unsigned long> >(seastar::reactor::write_some(seastar::pollable_fd_state&, void const*, unsigned long)::{lambda()#1}&&) 1 (0.00)
  - std::_Rb_tree<std::map<seastar::basic_sstring<char, unsigned int, 15u>, seastar::basic_sstring<char, unsigned int, 15u>, std::less<seastar::basic_sstring<char, unsigned int, 15u> >, std::allocator<std::pair<seastar::basic_sstring<char, unsigned int, 15u> const, seastar::basic_sstring<char, unsigned int, 15u> > > >, std::pair<std::map<seastar::basic_sstring<char, unsigned int, 15u>, seastar::basic_sstring<char, unsigned int, 15u>, std::less<seastar::basic_sstring<char, unsigned int, 15u> >, std::allocator<std::pair<seastar::basic_sstring<char, unsigned int, 15u> const, seastar::basic_sstring<char, unsigned int, 15u> > > > const, seastar::shared_ptr<seastar::metrics::impl::registered_metric> >, std::_Select1st<std::pair<std::map<seastar::basic_sstring<char, unsigned int, 15u>, seastar::basic_sstring<char, unsigned int, 15u>, std::less<seastar::basic_sstring<char, unsigned int, 15u> >, std::allocator<std::pair<seastar::basic_sstring<char, unsigned int, 15u> const, seastar::basic_sstring<char, unsigned int, 15u> > > > const, seastar::shared_ptr<seastar::metrics::impl::registered_metric> > >, std::less<std::map<seastar::basic_sstring<char, unsigned int, 15u>, seastar::basic_sstring<char, unsigned int, 15u>, std::less<seastar::basic_sstring<char, unsigned int, 15u> >, std::allocator<std::pair<seastar::basic_sstring<char, unsigned int, 15u> const, seastar::basic_sstring<char, unsigned int, 15u> > > > >, std::allocator<std::pair<std::map<seastar::basic_sstring<char, unsigned int, 15u>, seastar::basic_sstring<char, unsigned int, 15u>, std::less<seastar::basic_sstring<char, unsigned int, 15u> >, std::allocator<std::pair<seastar::basic_sstring<char, unsigned int, 15u> const, seastar::basic_sstring<char, unsigned int, 15u> > > > const, seastar::shared_ptr<seastar::metrics::impl::registered_metric> > > >::_M_lower_bound(std::_Rb_tree_node<std::pair<std::map<seastar::basic_sstring<char, unsigned int, 15u>, seastar::basic_sstring<char, unsigned int, 15u>, std::less<seastar::basic_sstring<char, unsigned int, 15u> >, std::allocator<std::pair<seastar::basic_sstring<char, unsigned int, 15u> const, seastar::basic_sstring<char, unsigned int, 15u> > > > const, seastar::shared_ptr<seastar::metrics::impl::registered_metric> > >*, std::_Rb_tree_node_base*, std::map<seastar::basic_sstring<char, unsigned int, 15u>, seastar::basic_sstring<char, unsigned int, 15u>, std::less<seastar::basic_sstring<char, unsigned int, 15u> >, std::allocator<std::pair<seastar::basic_sstring<char, unsigned int, 15u> const, seastar::basic_sstring<char, unsigned int, 15u> > > > const&) [clone .isra.513] 1 (0.00)
  - seastar::timer_set<seastar::timer<seastar::lowres_clock>, &seastar::timer<seastar::lowres_clock>::_link>::expire(std::chrono::time_point<seastar::lowres_clock, std::chrono::duration<long, std::ratio<1l, 1000l> > >) 1 (0.00)
  - cql::SessionConfiguration::getLogger() const & 1 (0.00)
  - seastar::reactor_backend_epoll::readable(seastar::pollable_fd_state&) 1 (0.00)
  - seastar::reactor::insert_activating_task_queues() 1 (0.00)
  - cql::ProtocolBatchParameters::encode(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&) const 1 (0.00)
  - seastar::reactor_backend_epoll::get_epoll_future(seastar::pollable_fd_state&, seastar::promise<> seastar::pollable_fd_state::*, int) 1 (0.00)
  - LZ4_compress_default 1 (0.00)
  - _ZN7seastar8internal8repeaterIZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSD_3_V212system_clockENSD_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS4_S6_S8_ENKUlvE2_clEvEUlvE_E15run_and_disposeEv 1 (0.00)
  - __read 1 (0.00)
  - seastar::future<>::set_callback(std::unique_ptr<seastar::continuation_base<>, std::default_delete<seastar::continuation_base<> > >) 1 (0.00)
  - cql::PrepareMessage::reset(cql::MessageHeader&&) 1 (0.00)
  - seastar::reactor::lowres_timer_pollfn::try_enter_interrupt_mode() 1 (0.00)
  - cql::Object<cql::BatchMessage> cql::RequestMessageFactory::makeRequestMessage<cql::BatchMessage>() 1 (0.00)
  - seastar::future<> seastar::future<>::then_wrapped<seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2}, seastar::future<> >(seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2}&&) 1 (0.00)
  - seastar::promise<>::abandoned() [clone .isra.195] 1 (0.00)
  - cql::Object<cql::ResultMessage> cql::makeObject<cql::ResultMessage, cql::MessageHeader>(cql::MessageHeader&&) 1 (0.00)
  - seastar::schedule_urgent(std::unique_ptr<seastar::task, std::default_delete<seastar::task> >) 1 (0.00)
  - seastar::continuation<seastar::future<seastar::temporary_buffer<char> > seastar::future<unsigned long>::then<seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}, seastar::future<seastar::temporary_buffer<char> > >(seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}&&)::{lambda(auto:1)#1}, unsigned long>::run_and_dispose() 1 (0.00)
  - fscanf 1 (0.00)
    - _IO_vfscanf 1 (0.00)
  - std::enable_if<!seastar::is_future<std::result_of<seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2} (seastar::future<>&&)>::type>::value, seastar::future<> >::type seastar::do_void_futurize_apply<seastar::output_stream<char>::poll_flush()::{lambda(seastar::future<>)#2}, seastar::future<> >(std::result_of&&, seastar::future<>&&) 1 (0.00)
  - cql::Session::batchExecute(cql::BatchCommand&&) 1 (0.00)
  - cql::ResultMessage::decodeBody(cql::ConnectionInfo const&, seastar::temporary_buffer<char>&&) 1 (0.00)
  - seastar::promise<>::abandoned() [clone .isra.340] 1 (0.00)
  - seastar::internal::do_with_state<std::tuple<seastar::lw_shared_ptr<cql::Connection>, cql::Object<cql::ResponseMessageBase> >, seastar::future<> >::run_and_dispose() 1 (0.00)
  - cql::MessageBase::getHeader() & 1 (0.00)
```

# Performance analysis (perf)

Provided by linux perf tool.<br/>
Last updated version: 0.2<br/>
Last analysis target: batch insert +p+c single core<br/>

``` text
# Samples: 1K of event 'cpu-clock'
# Event count (approx.): 336250000
#
# Overhead  Command    Shared Object        Symbol                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
# ........  .........  ...................  ............................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................
#
    11.08%  benchmark  [kernel.kallsyms]    [k] entry_SYSCALL_64_fastpath
     8.77%  benchmark  libc-2.26.so         [.] epoll_pwait
     4.68%  reactor-2  [kernel.kallsyms]    [k] native_write_msr
     4.54%  benchmark  [kernel.kallsyms]    [k] _raw_spin_unlock_irqrestore
     4.54%  reactor-1  [kernel.kallsyms]    [k] native_write_msr
     4.24%  benchmark  [kernel.kallsyms]    [k] __softirqentry_text_start
     2.83%  benchmark  benchmark            [.] seastar::smp::poll_queues
     2.30%  reactor-2  [kernel.kallsyms]    [k] entry_SYSCALL_64_fastpath
     1.93%  benchmark  [kernel.kallsyms]    [k] sys_epoll_pwait
     1.71%  benchmark  [kernel.kallsyms]    [k] __fget
     1.56%  reactor-1  [kernel.kallsyms]    [k] entry_SYSCALL_64_fastpath
     1.41%  benchmark  [kernel.kallsyms]    [k] finish_task_switch
     1.26%  benchmark  [vdso]               [.] 0x0000000000000979
     1.26%  benchmark  benchmark            [.] seastar::reactor::run
     1.12%  benchmark  benchmark            [.] seastar::smp_message_queue::flush_response_batch
     1.12%  benchmark  benchmark            [.] std::_Function_handler<bool (), seastar::reactor::run()::{lambda()#7}>::_M_invoke
     1.04%  reactor-1  [kernel.kallsyms]    [k] do_timerfd_settime
     0.97%  benchmark  benchmark            [.] seastar::internal::io_getevents
     0.82%  benchmark  benchmark            [.] seastar::smp_message_queue::process_queue<4ul, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}>
     0.74%  benchmark  benchmark            [.] seastar::reactor::signals::action
     0.74%  benchmark  benchmark            [.] seastar::smp_message_queue::process_queue<2ul, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}>
     0.67%  benchmark  [kernel.kallsyms]    [k] fput
     0.67%  benchmark  [vdso]               [.] __vdso_clock_gettime
     0.67%  reactor-2  libc-2.26.so         [.] timerfd_settime
     0.59%  benchmark  [kernel.kallsyms]    [k] __fget_light
     0.59%  benchmark  [kernel.kallsyms]    [k] ep_poll
     0.59%  reactor-2  [kernel.kallsyms]    [k] __softirqentry_text_start
     0.59%  reactor-2  [kernel.kallsyms]    [k] clear_page_orig
     0.52%  benchmark  benchmark            [.] seastar::reactor::sleep
     0.52%  benchmark  libc-2.26.so         [.] __libc_disable_asynccancel
     0.52%  reactor-1  [kernel.kallsyms]    [k] clear_page_orig
     0.52%  reactor-2  [kernel.kallsyms]    [k] do_timerfd_settime
     0.45%  benchmark  [kernel.kallsyms]    [k] clear_page_orig
     0.45%  benchmark  [kernel.kallsyms]    [k] copy_user_generic_unrolled
     0.45%  benchmark  benchmark            [.] seastar::reactor_backend_epoll::wait_and_process
     0.45%  reactor-1  [kernel.kallsyms]    [k] __x2apic_send_IPI_mask
     0.45%  reactor-1  libc-2.26.so         [.] timerfd_settime
     0.45%  reactor-3  [kernel.kallsyms]    [k] _raw_spin_unlock_irqrestore
     0.45%  reactor-3  [kernel.kallsyms]    [k] clear_page_orig
     0.37%  benchmark  benchmark            [.] seastar::reactor::flush_tcp_batches
     0.37%  benchmark  benchmark            [.] seastar::reactor::io_pollfn::poll
     0.37%  benchmark  benchmark            [.] seastar::syscall_work_queue::complete
     0.37%  reactor-2  [kernel.kallsyms]    [k] copy_page_regs
     0.37%  reactor-2  libc-2.26.so         [.] __memmove_avx_unaligned_erms
     0.37%  reactor-2  libpthread-2.26.so   [.] pthread_sigmask
     0.30%  benchmark  [kernel.kallsyms]    [k] handle_mm_fault
     0.30%  benchmark  [kernel.kallsyms]    [k] native_write_msr
     0.30%  benchmark  benchmark            [.] _ZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSA_3_V212system_clockENSA_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS1_S3_S5_ENKUlvE2_clEvENKUlvE_clEv
     0.30%  benchmark  benchmark            [.] seastar::basic_semaphore<seastar::semaphore_default_exception_factory, std::chrono::_V2::steady_clock>::signal
     0.30%  benchmark  benchmark            [.] seastar::reactor::do_expire_lowres_timers
     0.30%  benchmark  benchmark            [.] seastar::reactor::flush_pending_aio
     0.30%  benchmark  libc-2.26.so         [.] __memmove_avx_unaligned_erms
     0.30%  reactor-1  [kernel.kallsyms]    [k] _raw_spin_unlock_irqrestore
     0.30%  reactor-1  [kernel.kallsyms]    [k] copy_page_regs
     0.30%  reactor-1  [kernel.kallsyms]    [k] copy_user_generic_unrolled
     0.30%  reactor-1  libpthread-2.26.so   [.] pthread_sigmask
     0.30%  reactor-2  [kernel.kallsyms]    [k] rwsem_spin_on_owner
     0.30%  reactor-3  libc-2.26.so         [.] __memmove_avx_unaligned_erms
     0.22%  benchmark  [kernel.kallsyms]    [k] __set_current_blocked
     0.22%  benchmark  [kernel.kallsyms]    [k] _raw_spin_lock_irqsave
     0.22%  benchmark  [kernel.kallsyms]    [k] exit_to_usermode_loop
     0.22%  benchmark  [kernel.kallsyms]    [k] get_mem_cgroup_from_mm
     0.22%  benchmark  [kernel.kallsyms]    [k] kmem_cache_alloc
     0.22%  benchmark  [kernel.kallsyms]    [k] perf_iterate_ctx
     0.22%  benchmark  [kernel.kallsyms]    [k] sys_rt_sigprocmask
     0.22%  benchmark  [vdso]               [.] 0x000000000000098c
     0.22%  benchmark  benchmark            [.] _ZZZN3cql7Session12batchExecuteEONS_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS1_N7seastar13lw_shared_ptrINS_14ConnectionPoolEEENS_12_GLOBAL__N_19RetryFlowENSK_INS_10ConnectionEEENS_16ConnectionStreamENS_6ObjectINS_12BatchMessageEEENSS_INSN_14PrepareResultsEEEEEDaS4_S6_S8_SA_SC_SE_SG_ENKUlvE_clEv
     0.22%  benchmark  benchmark            [.] seastar::reactor::lowres_timer_pollfn::exit_interrupt_mode
     0.22%  benchmark  benchmark            [.] seastar::reactor::process_io
     0.22%  benchmark  libc-2.26.so         [.] __clock_gettime
     0.22%  benchmark  libc-2.26.so         [.] __libc_enable_asynccancel
     0.22%  benchmark  libc-2.26.so         [.] sigfillset
     0.22%  benchmark  libstdc++.so.6.0.24  [.] std::chrono::_V2::steady_clock::now
     0.22%  reactor-1  [kernel.kallsyms]    [k] __fget
     0.22%  reactor-1  [kernel.kallsyms]    [k] smp_call_function_single
     0.22%  reactor-1  libc-2.26.so         [.] __memmove_avx_unaligned_erms
     0.22%  reactor-2  benchmark            [.] seastar::smp::poll_queues
     0.22%  reactor-2  benchmark            [.] seastar::smp_message_queue::flush_response_batch
     0.22%  reactor-3  [kernel.kallsyms]    [k] entry_SYSCALL_64_fastpath
     0.22%  reactor-3  [kernel.kallsyms]    [k] flush_tlb_mm_range
     0.22%  reactor-3  [kernel.kallsyms]    [k] native_write_msr
     0.22%  reactor-3  [kernel.kallsyms]    [k] rwsem_spin_on_owner
     0.22%  reactor-3  [vdso]               [.] 0x0000000000000979
     0.15%  benchmark  [kernel.kallsyms]    [k] __clear_user
     0.15%  benchmark  [kernel.kallsyms]    [k] __do_page_fault
     0.15%  benchmark  [kernel.kallsyms]    [k] __fdget
     0.15%  benchmark  [kernel.kallsyms]    [k] __nf_ct_l4proto_find
     0.15%  benchmark  [kernel.kallsyms]    [k] __vma_adjust
     0.15%  benchmark  [kernel.kallsyms]    [k] __x86_indirect_thunk_rax
     0.15%  benchmark  [kernel.kallsyms]    [k] _copy_from_user
     0.15%  benchmark  [kernel.kallsyms]    [k] find_vma
     0.15%  benchmark  [kernel.kallsyms]    [k] get_signal
     0.15%  benchmark  [kernel.kallsyms]    [k] vfs_read
     0.15%  benchmark  benchmark            [.] cql::BatchCommand::openParameterSet
     0.15%  benchmark  benchmark            [.] cql::BatchMessage::encodeBody
     0.15%  benchmark  benchmark            [.] cql::ConnectionStream::~ConnectionStream
     0.15%  benchmark  benchmark            [.] seastar::memory::cpu_pages::drain_cross_cpu_freelist
     0.15%  benchmark  benchmark            [.] seastar::memory::cpu_pages::free
     0.15%  benchmark  benchmark            [.] seastar::memory::small_pool::allocate
     0.15%  benchmark  benchmark            [.] seastar::reactor::del_timer
     0.15%  benchmark  benchmark            [.] seastar::reactor::signal_pollfn::exit_interrupt_mode
     0.15%  benchmark  benchmark            [.] seastar::reactor::signals::poll_signal
     0.15%  benchmark  benchmark            [.] seastar::reactor::smp_pollfn::poll
     0.15%  benchmark  benchmark            [.] seastar::systemwide_memory_barrier
     0.15%  benchmark  benchmark            [.] std::_Function_handler<void (), seastar::reactor::run()::{lambda()#5}>::_M_invoke
     0.15%  benchmark  benchmark            [.] std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_mutate
     0.15%  benchmark  liblz4.so.1.7.1      [.] LZ4_compress_fast_extState
     0.15%  benchmark  libpthread-2.26.so   [.] pthread_sigmask
     0.15%  benchmark  librt-2.26.so        [.] timer_settime@@GLIBC_2.3.3
     0.15%  benchmark  libstdc++.so.6.0.24  [.] std::chrono::_V2::system_clock::now
     0.15%  reactor-1  [kernel.kallsyms]    [k] exit_to_usermode_loop
     0.15%  reactor-1  [vdso]               [.] __vdso_clock_gettime
     0.15%  reactor-1  benchmark            [.] seastar::reactor::run
     0.15%  reactor-1  benchmark            [.] seastar::smp_message_queue::process_queue<4ul, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}>
     0.15%  reactor-2  [kernel.kallsyms]    [k] __set_current_blocked
     0.15%  reactor-2  [kernel.kallsyms]    [k] __x2apic_send_IPI_mask
     0.15%  reactor-2  [kernel.kallsyms]    [k] copy_user_generic_unrolled
     0.15%  reactor-2  [kernel.kallsyms]    [k] exit_to_usermode_loop
     0.15%  reactor-2  [kernel.kallsyms]    [k] migrate_page_move_mapping
     0.15%  reactor-2  benchmark            [.] seastar::reactor::execution_stage_pollfn::poll
     0.15%  reactor-2  benchmark            [.] seastar::reactor::io_pollfn::poll
     0.15%  reactor-2  benchmark            [.] seastar::reactor::run
     0.15%  reactor-2  benchmark            [.] seastar::smp_message_queue::process_queue<2ul, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}>
     0.15%  reactor-2  benchmark            [.] seastar::syscall_work_queue::complete
     0.15%  reactor-3  [kernel.kallsyms]    [k] __do_page_fault
     0.15%  reactor-3  [kernel.kallsyms]    [k] do_mprotect_pkey
     0.15%  reactor-3  [kernel.kallsyms]    [k] perf_iterate_ctx
     0.15%  reactor-3  [vdso]               [.] __vdso_clock_gettime
     0.15%  reactor-3  benchmark            [.] seastar::smp::poll_queues
     0.15%  reactor-3  benchmark            [.] std::_Function_handler<bool (), seastar::reactor::run()::{lambda()#7}>::_M_invoke
     0.15%  reactor-3  libc-2.26.so         [.] __mprotect
     0.15%  reactor-3  libpthread-2.26.so   [.] pthread_sigmask
     0.07%  benchmark  [kernel.kallsyms]    [k] __copy_skb_header
     0.07%  benchmark  [kernel.kallsyms]    [k] __d_lookup
     0.07%  benchmark  [kernel.kallsyms]    [k] __kmalloc_node_track_caller
     0.07%  benchmark  [kernel.kallsyms]    [k] __rb_insert_augmented
     0.07%  benchmark  [kernel.kallsyms]    [k] __split_vma
     0.07%  benchmark  [kernel.kallsyms]    [k] __tcp_select_window
     0.07%  benchmark  [kernel.kallsyms]    [k] __x2apic_send_IPI_mask
     0.07%  benchmark  [kernel.kallsyms]    [k] aa_sk_perm
     0.07%  benchmark  [kernel.kallsyms]    [k] change_protection_range
     0.07%  benchmark  [kernel.kallsyms]    [k] copyout
     0.07%  benchmark  [kernel.kallsyms]    [k] cpumask_any_but
     0.07%  benchmark  [kernel.kallsyms]    [k] do_signal
     0.07%  benchmark  [kernel.kallsyms]    [k] do_timer_settime
     0.07%  benchmark  [kernel.kallsyms]    [k] filemap_map_pages
     0.07%  benchmark  [kernel.kallsyms]    [k] flush_tlb_mm_range
     0.07%  benchmark  [kernel.kallsyms]    [k] free_hot_cold_page
     0.07%  benchmark  [kernel.kallsyms]    [k] free_pages_and_swap_cache
     0.07%  benchmark  [kernel.kallsyms]    [k] ip_rcv
     0.07%  benchmark  [kernel.kallsyms]    [k] ipt_do_table
     0.07%  benchmark  [kernel.kallsyms]    [k] kmem_cache_free
     0.07%  benchmark  [kernel.kallsyms]    [k] link_path_walk
     0.07%  benchmark  [kernel.kallsyms]    [k] may_open
     0.07%  benchmark  [kernel.kallsyms]    [k] memcg_kmem_get_cache
     0.07%  benchmark  [kernel.kallsyms]    [k] mprotect_fixup
     0.07%  benchmark  [kernel.kallsyms]    [k] nf_hook_slow
     0.07%  benchmark  [kernel.kallsyms]    [k] page_cpupid_xchg_last
     0.07%  benchmark  [kernel.kallsyms]    [k] page_remove_rmap
     0.07%  benchmark  [kernel.kallsyms]    [k] perf_event_mmap
     0.07%  benchmark  [kernel.kallsyms]    [k] perf_event_mmap_output
     0.07%  benchmark  [kernel.kallsyms]    [k] perf_iterate_sb
     0.07%  benchmark  [kernel.kallsyms]    [k] queue_work_on
     0.07%  benchmark  [kernel.kallsyms]    [k] scsi_finish_command
     0.07%  benchmark  [kernel.kallsyms]    [k] setup_sigcontext
     0.07%  benchmark  [kernel.kallsyms]    [k] show_cpuinfo
     0.07%  benchmark  [kernel.kallsyms]    [k] sk_reset_timer
     0.07%  benchmark  [kernel.kallsyms]    [k] tcp_ack
     0.07%  benchmark  [kernel.kallsyms]    [k] tcp_pkt_to_tuple
     0.07%  benchmark  [kernel.kallsyms]    [k] tcp_poll
     0.07%  benchmark  [kernel.kallsyms]    [k] tcp_stream_memory_free
     0.07%  benchmark  [kernel.kallsyms]    [k] unmap_page_range
     0.07%  benchmark  [kernel.kallsyms]    [k] uprobe_deny_signal
     0.07%  benchmark  [kernel.kallsyms]    [k] vm_normal_page
     0.07%  benchmark  [kernel.kallsyms]    [k] vma_adjust_trans_huge
     0.07%  benchmark  [vdso]               [.] 0x0000000000000970
     0.07%  benchmark  [vdso]               [.] 0x000000000000097d
     0.07%  benchmark  benchmark            [.] _ZN7seastar12continuationIZNS_6futureIJEE4thenIZZZZZ4mainENKUlvE_clEvENKUlRT_RT0_RT1_E_clIN3cql7SessionENSt6chrono10time_pointINSF_3_V212system_clockENSF_8durationIlSt5ratioILl1ELl1000000000EEEEEEmEEDaS6_S8_SA_ENKUlvE2_clEvENKUlvE_clEvEUlvE_NS1_IJNS_10bool_classINS_18stop_iteration_tagEEEEEEEES7_OS5_EUlSV_E_JEE15run_and_disposeEv
     0.07%  benchmark  benchmark            [.] _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedINS5_12finally_bodyIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlvE6_Lb0EEES5_EESE_OSC_EUlS18_E_JS4_EE15run_and_disposeEv
     0.07%  benchmark  benchmark            [.] _ZN7seastar12continuationIZNS_6futureIJNS_10bool_classINS_18stop_iteration_tagEEEEE12then_wrappedIZNS5_16handle_exceptionIZZZN3cql7Session12batchExecuteEONS8_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clISA_NS_13lw_shared_ptrINS8_14ConnectionPoolEEENS8_12_GLOBAL__N_19RetryFlowENSS_INS8_10ConnectionEEENS8_16ConnectionStreamENS8_6ObjectINS8_12BatchMessageEEENS10_INSV_14PrepareResultsEEEEEDaSD_SF_SH_SJ_SL_SN_SP_ENKUlvE_clEvEUlNSt15__exception_ptr13exception_ptrEE5_EES5_OSC_EUlS19_E_S5_EESE_S19_EUlS19_E_JS4_EE15run_and_disposeEv
     0.07%  benchmark  benchmark            [.] _ZN7seastar8futurizeINS_10bool_classINS_18stop_iteration_tagEEEE5applyIZZZN3cql10Connection15waitNextMessageERKNS6_16ConnectionStreamEENKUlRT_RT0_E_clINS_13lw_shared_ptrIS7_EENS6_6ObjectINS6_19ResponseMessageBaseEEEEEDaSC_SE_ENKUlvE_clEvEUlONS_16temporary_bufferIcEEE0_JSO_EEENS_6futureIJS3_EEEOSB_OSt5tupleIJDpT0_EE
     0.07%  benchmark  benchmark            [.] _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlS9_E3_JNSX_INS5_19ResponseMessageBaseEEEEEES2_OS9_OSt5tupleIJDpT0_EE
     0.07%  benchmark  benchmark            [.] _ZN7seastar8futurizeINS_6futureIJEEEE5applyIZZZN3cql7Session12batchExecuteEONS5_12BatchCommandEENKUlRT_RT0_RT1_RT2_RT3_RT4_RT5_E_clIS7_NS_13lw_shared_ptrINS5_14ConnectionPoolEEENS5_12_GLOBAL__N_19RetryFlowENSP_INS5_10ConnectionEEENS5_16ConnectionStreamENS5_6ObjectINS5_12BatchMessageEEENSX_INSS_14PrepareResultsEEEEEDaSA_SC_SE_SG_SI_SK_SM_ENKUlvE_clEvEUlvE0_JEEES2_OS9_OSt5tupleIJDpT0_EE.isra.704
     0.07%  benchmark  benchmark            [.] __errno_location@plt
     0.07%  benchmark  benchmark            [.] __tls_init
     0.07%  benchmark  benchmark            [.] boost::intrusive::circular_list_algorithms<boost::intrusive::list_node_traits<void*> >::swap_nodes
     0.07%  benchmark  benchmark            [.] cql::BatchCommand::getParameterCountOfLastSet
     0.07%  benchmark  benchmark            [.] cql::Connection::sendMessage
     0.07%  benchmark  benchmark            [.] cql::Connection::waitNextMessage
     0.07%  benchmark  benchmark            [.] cql::ConnectionStream::ConnectionStream
     0.07%  benchmark  benchmark            [.] cql::MessageHeader::MessageHeader
     0.07%  benchmark  benchmark            [.] cql::NodeConfiguration::getPreparedQueryId[abi:cxx11]
     0.07%  benchmark  benchmark            [.] cql::Object<cql::BatchCommandData>::Object(std::unique_ptr<cql::BatchCommandData, std::default_delete<cql::BatchCommandData> >&&)::{lambda(void*)#1}::_FUN
     0.07%  benchmark  benchmark            [.] cql::ProtocolResultPreparedMetadata::reset
     0.07%  benchmark  benchmark            [.] seastar::continuation_base<seastar::temporary_buffer<char> >::~continuation_base
     0.07%  benchmark  benchmark            [.] seastar::data_sink_impl::allocate_buffer
     0.07%  benchmark  benchmark            [.] seastar::future<>::forward_to
     0.07%  benchmark  benchmark            [.] seastar::future<seastar::bool_class<seastar::stop_iteration_tag> >::forward_to
     0.07%  benchmark  benchmark            [.] seastar::future<seastar::temporary_buffer<char> > seastar::future<unsigned long>::then<seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}, seastar::future<seastar::temporary_buffer<char> > >(seastar::net::posix_data_source_impl::get()::{lambda(unsigned long)#1}&&)::{lambda(auto:1)#1}::~then
     0.07%  benchmark  benchmark            [.] seastar::future_state<unsigned long>::operator=
     0.07%  benchmark  benchmark            [.] seastar::memory::allocate
     0.07%  benchmark  benchmark            [.] seastar::memory::cpu_pages::allocate_small
     0.07%  benchmark  benchmark            [.] seastar::memory::drain_cross_cpu_freelist
     0.07%  benchmark  benchmark            [.] seastar::output_stream<char>::poll_flush
     0.07%  benchmark  benchmark            [.] seastar::output_stream<char>::write
     0.07%  benchmark  benchmark            [.] seastar::promise<>::abandoned
     0.07%  benchmark  benchmark            [.] seastar::reactor::add_urgent_task
     0.07%  benchmark  benchmark            [.] seastar::reactor::aio_batch_submit_pollfn::poll
     0.07%  benchmark  benchmark            [.] seastar::reactor::batch_flush_pollfn::poll
     0.07%  benchmark  benchmark            [.] seastar::reactor::drain_cross_cpu_freelist_pollfn::poll
     0.07%  benchmark  benchmark            [.] seastar::reactor::drain_cross_cpu_freelist_pollfn::try_enter_interrupt_mode
     0.07%  benchmark  benchmark            [.] seastar::reactor::epoll_pollfn::exit_interrupt_mode
     0.07%  benchmark  benchmark            [.] seastar::reactor::epoll_pollfn::poll
     0.07%  benchmark  benchmark            [.] seastar::reactor::execution_stage_pollfn::poll
     0.07%  benchmark  benchmark            [.] seastar::reactor::signal_pollfn::poll
     0.07%  benchmark  benchmark            [.] seastar::reactor_backend_epoll::complete_epoll_event
     0.07%  benchmark  benchmark            [.] seastar::reactor_backend_epoll::get_epoll_future
     0.07%  benchmark  benchmark            [.] std::_Function_handler<seastar::reactor::idle_cpu_handler_result (std::function<bool ()> const&), seastar::reactor::_idle_cpu_handler::{lambda(std::function<bool ()> const&)#1}>::_M_invoke
     0.07%  benchmark  benchmark            [.] std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_assign
     0.07%  benchmark  benchmark            [.] std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace_aux
     0.07%  benchmark  benchmark            [.] std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::resize
     0.07%  benchmark  benchmark            [.] std::chrono::_V2::steady_clock::now@plt
     0.07%  benchmark  benchmark            [.] timer_settime@plt
     0.07%  benchmark  ld-2.26.so           [.] _dl_relocate_object
     0.07%  benchmark  ld-2.26.so           [.] check_match
     0.07%  benchmark  ld-2.26.so           [.] strcmp
     0.07%  benchmark  libc-2.26.so         [.] __fxstat64
     0.07%  benchmark  libc-2.26.so         [.] __getpagesize
     0.07%  benchmark  libc-2.26.so         [.] __memset_avx2_erms
     0.07%  benchmark  libc-2.26.so         [.] __memset_avx2_unaligned_erms
     0.07%  benchmark  libc-2.26.so         [.] __mprotect
     0.07%  benchmark  libpthread-2.26.so   [.] __libc_read
     0.07%  benchmark  libpthread-2.26.so   [.] __libc_send
     0.07%  reactor-1  [kernel.kallsyms]    [k] __do_page_fault
     0.07%  reactor-1  [kernel.kallsyms]    [k] __set_current_blocked
     0.07%  reactor-1  [kernel.kallsyms]    [k] __softirqentry_text_start
     0.07%  reactor-1  [kernel.kallsyms]    [k] fput
     0.07%  reactor-1  [kernel.kallsyms]    [k] free_hot_cold_page
     0.07%  reactor-1  [kernel.kallsyms]    [k] get_itimerspec64
     0.07%  reactor-1  [kernel.kallsyms]    [k] isolate_freepages_block
     0.07%  reactor-1  [kernel.kallsyms]    [k] memcpy_orig
     0.07%  reactor-1  [kernel.kallsyms]    [k] putback_lru_page
     0.07%  reactor-1  [kernel.kallsyms]    [k] sys_rt_sigprocmask
     0.07%  reactor-1  [kernel.kallsyms]    [k] total_mapcount
     0.07%  reactor-1  [kernel.kallsyms]    [k] unmapped_area_topdown
     0.07%  reactor-1  [kernel.kallsyms]    [k] vma_interval_tree_subtree_search
     0.07%  reactor-1  [vdso]               [.] 0x0000000000000977
     0.07%  reactor-1  benchmark            [.] boost::algorithm::replace_all_copy<seastar::basic_sstring<char, unsigned int, 15u>, char [2], char [2]>
     0.07%  reactor-1  benchmark            [.] seastar::internal::io_getevents
     0.07%  reactor-1  benchmark            [.] seastar::reactor::drain_cross_cpu_freelist_pollfn::poll
     0.07%  reactor-1  benchmark            [.] seastar::reactor::flush_tcp_batches
     0.07%  reactor-1  benchmark            [.] seastar::reactor::signal_pollfn::try_enter_interrupt_mode
     0.07%  reactor-1  benchmark            [.] seastar::reactor::syscall_pollfn::poll
     0.07%  reactor-1  benchmark            [.] seastar::smp::poll_queues
     0.07%  reactor-1  benchmark            [.] seastar::smp_message_queue::process_queue<2ul, seastar::smp_message_queue::process_incoming()::{lambda(seastar::smp_message_queue::work_item*)#1}>
     0.07%  reactor-1  benchmark            [.] seastar::syscall_work_queue::complete
     0.07%  reactor-1  benchmark            [.] std::_Function_handler<bool (), seastar::reactor::run()::{lambda()#7}>::_M_invoke
     0.07%  reactor-1  libstdc++.so.6.0.24  [.] std::chrono::_V2::steady_clock::now
     0.07%  reactor-2  [kernel.kallsyms]    [k] __pageblock_pfn_to_page
     0.07%  reactor-2  [kernel.kallsyms]    [k] _copy_from_user
     0.07%  reactor-2  [kernel.kallsyms]    [k] _raw_spin_unlock_irqrestore
     0.07%  reactor-2  [kernel.kallsyms]    [k] dec_node_page_state
     0.07%  reactor-2  [kernel.kallsyms]    [k] get_page_from_freelist
     0.07%  reactor-2  [kernel.kallsyms]    [k] kmem_cache_alloc
     0.07%  reactor-2  [kernel.kallsyms]    [k] mpt_put_msg_frame
     0.07%  reactor-2  [kernel.kallsyms]    [k] page_remove_rmap
     0.07%  reactor-2  [kernel.kallsyms]    [k] queue_work_on
     0.07%  reactor-2  [kernel.kallsyms]    [k] sigprocmask
     0.07%  reactor-2  [kernel.kallsyms]    [k] sys_timerfd_settime
     0.07%  reactor-2  [kernel.kallsyms]    [k] vma_interval_tree_iter_next
     0.07%  reactor-2  [vdso]               [.] __vdso_clock_gettime
     0.07%  reactor-2  [vdso]               [.] 0x0000000000000979
     0.07%  reactor-2  benchmark            [.] seastar::memory::cpu_pages::allocate_small
     0.07%  reactor-2  benchmark            [.] seastar::reactor::flush_tcp_batches
     0.07%  reactor-2  benchmark            [.] seastar::reactor::signals::poll_signal
     0.07%  reactor-2  benchmark            [.] seastar::reactor::syscall_pollfn::poll
     0.07%  reactor-2  benchmark            [.] seastar::smp_message_queue::process_queue<4ul, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}>
     0.07%  reactor-2  ld-2.26.so           [.] memset
     0.07%  reactor-2  libc-2.26.so         [.] __mprotect
     0.07%  reactor-3  [kernel.kallsyms]    [k] __fget_light
     0.07%  reactor-3  [kernel.kallsyms]    [k] __split_vma
     0.07%  reactor-3  [kernel.kallsyms]    [k] __vma_adjust
     0.07%  reactor-3  [kernel.kallsyms]    [k] __x2apic_send_IPI_mask
     0.07%  reactor-3  [kernel.kallsyms]    [k] _copy_from_user
     0.07%  reactor-3  [kernel.kallsyms]    [k] anon_vma_clone
     0.07%  reactor-3  [kernel.kallsyms]    [k] copy_user_generic_unrolled
     0.07%  reactor-3  [kernel.kallsyms]    [k] do_timerfd_settime
     0.07%  reactor-3  [kernel.kallsyms]    [k] exit_to_usermode_loop
     0.07%  reactor-3  [kernel.kallsyms]    [k] queue_work_on
     0.07%  reactor-3  [kernel.kallsyms]    [k] security_mmap_file
     0.07%  reactor-3  [kernel.kallsyms]    [k] sys_timerfd_settime
     0.07%  reactor-3  [kernel.kallsyms]    [k] vma_merge
     0.07%  reactor-3  benchmark            [.] getpagesize@plt
     0.07%  reactor-3  benchmark            [.] seastar::internal::io_getevents
     0.07%  reactor-3  benchmark            [.] seastar::reactor::aio_batch_submit_pollfn::poll
     0.07%  reactor-3  benchmark            [.] seastar::reactor::io_pollfn::poll
     0.07%  reactor-3  benchmark            [.] seastar::reactor::run
     0.07%  reactor-3  benchmark            [.] seastar::smp_message_queue::process_queue<4ul, seastar::smp_message_queue::process_completions()::{lambda(seastar::smp_message_queue::work_item*)#1}>
     0.07%  reactor-3  benchmark            [.] seastar::systemwide_memory_barrier
     0.07%  syscall-1  [kernel.kallsyms]    [k] unmap_page_range
     0.07%  timer-1    [kernel.kallsyms]    [k] _raw_spin_unlock_irqrestore
     0.07%  timer-1    [kernel.kallsyms]    [k] clear_page_orig
     0.07%  timer-2    [kernel.kallsyms]    [k] clear_page_orig
     0.07%  timer-3    [kernel.kallsyms]    [k] _raw_spin_unlock_irqrestore
```

