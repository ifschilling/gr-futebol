#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: IEEE 802.15.4 Transceiver using OQPSK PHY
# Generated: Thu Apr 18 15:44:51 2019
##################################################


import os
import sys
sys.path.append(os.environ.get('GRC_HIER_PATH', os.path.expanduser('~/.grc_gnuradio')))

from RSSI import RSSI  # grc-generated hier_block
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from ieee802_15_4_oqpsk_phy import ieee802_15_4_oqpsk_phy  # grc-generated hier_block
from optparse import OptionParser
import foo
import futebol
import ieee802_15_4
import pmt


class transceiver(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "IEEE 802.15.4 Transceiver using OQPSK PHY")

        ##################################################
        # Variables
        ##################################################
        self.tx_gain = tx_gain = 0.75
        self.rx_gain = rx_gain = 0.75
        self.freq = freq = 2480000000

        ##################################################
        # Blocks
        ##################################################
        self.ieee802_15_4_rime_stack_0 = ieee802_15_4.rime_stack(([129]), ([131]), ([132]), ([23,42]))
        self.ieee802_15_4_oqpsk_phy_0 = ieee802_15_4_oqpsk_phy()
        self.ieee802_15_4_mac_0 = ieee802_15_4.mac(True,0x8841,0,0x1aaa,0xffff,0x3344)
        self.futebol_msg_to_file_0 = futebol.msg_to_file()
        self.futebol_msg_rssi_msg_0 = futebol.msg_rssi_msg(-0.5)
        self.foo_packet_pad_0 = foo.packet_pad(False, False, 0.001, 2000, 2000)
        self.blocks_message_strobe_0 = blocks.message_strobe(pmt.intern("Hello World!\n"), 10)
        self.blocks_file_sink_0_0 = blocks.file_sink(gr.sizeof_char*1, '/tmp/teste.csv', False)
        self.blocks_file_sink_0_0.set_unbuffered(True)
        self.RSSI_0 = RSSI()

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0, 'strobe'), (self.ieee802_15_4_rime_stack_0, 'bcin'))
        self.msg_connect((self.futebol_msg_rssi_msg_0, 'msg_out'), (self.futebol_msg_to_file_0, 'in'))
        self.msg_connect((self.ieee802_15_4_mac_0, 'pdu out'), (self.ieee802_15_4_oqpsk_phy_0, 'txin'))
        self.msg_connect((self.ieee802_15_4_mac_0, 'app out'), (self.ieee802_15_4_rime_stack_0, 'fromMAC'))
        self.msg_connect((self.ieee802_15_4_oqpsk_phy_0, 'rxout'), (self.futebol_msg_rssi_msg_0, 'msg_in'))
        self.msg_connect((self.ieee802_15_4_oqpsk_phy_0, 'rxout'), (self.ieee802_15_4_mac_0, 'pdu in'))
        self.msg_connect((self.ieee802_15_4_rime_stack_0, 'toMAC'), (self.ieee802_15_4_mac_0, 'app in'))
        self.connect((self.RSSI_0, 0), (self.futebol_msg_rssi_msg_0, 0))
        self.connect((self.foo_packet_pad_0, 0), (self.RSSI_0, 0))
        self.connect((self.foo_packet_pad_0, 0), (self.ieee802_15_4_oqpsk_phy_0, 0))
        self.connect((self.futebol_msg_to_file_0, 0), (self.blocks_file_sink_0_0, 0))
        self.connect((self.ieee802_15_4_oqpsk_phy_0, 0), (self.foo_packet_pad_0, 0))

    def get_tx_gain(self):
        return self.tx_gain

    def set_tx_gain(self, tx_gain):
        self.tx_gain = tx_gain

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq


def main(top_block_cls=transceiver, options=None):
    if gr.enable_realtime_scheduling() != gr.RT_OK:
        print "Error: failed to enable real-time scheduling."

    tb = top_block_cls()
    tb.start()
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main()
