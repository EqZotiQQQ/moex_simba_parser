This is SIMBA SPECTRA parser that decodes input binary data.
This lib should get PCAP dump files on input.
PCAP contains samples of SIMBA SPECTRA traffic captured on production system.


pipeline:

[global_pcap_parser] ->     Here we parse global pcap header

[udp_parser] ->             Here we parse ip and udp headers

[market_data_packet] ->     Here we start to parse Simba objects

[snapshot_packet] or [incremental_packet] -> Marked data packet contains snapshot or incremental packet

[sbe_message] -> snapshot and incremental packets contain SBE Message

[order_best_prices] or [order_book_snapshot] or [order_execution] or [order_update] -> Every SBE Message contains on of this packets (there is much more packets but those four were interesting for me)