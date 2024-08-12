"use client"

import { useState, useEffect } from 'react';
import OrderBook from '@/app/OrderBook';
import { io } from "socket.io-client";

const LandingPage = () => {
  const [bids, setBids] = useState([]);
  const [asks, setAsks] = useState([]);

  // handle incoming order or match
  const delta_bids = (price: number, volume: number) => {
        console.log("incoming bid:" + price + " " + volume)
        setBids((curr_bids) => {
          const index = curr_bids.findIndex((elem) => elem[0] == price);
          if (index > -1) {
            // Update the volume at the existing price point
            const updatedOrderBook = [...curr_bids];
            updatedOrderBook[index] = [price, updatedOrderBook[index][1] + volume];
            return updatedOrderBook;
        } else {
          // Insert new price point
          const updatedOrderBook = [...curr_bids, [price, volume]]
          return updatedOrderBook;
        }
        })
  };

  const delta_asks = (price: number, volume: number) => {
    console.log("incoming ask:" + price + " " + volume)
    setAsks((curr_asks) => {
      const index = curr_asks.findIndex((elem) => elem[0] == price);
      if (index > -1) {
        // Update the volume at the existing price point
        let updatedOrderBook = [...curr_asks];
        updatedOrderBook[index] = [price, updatedOrderBook[index][1] + volume];
        return updatedOrderBook;
    } else {
      // Insert new price point
      const updatedOrderBook = [...curr_asks, [price, volume]]
      return updatedOrderBook;
    }
    })
};

  let socket;
  useEffect(() => {
    const socketInitializer = async () => {
      await fetch('/api/socket')
      socket = io()
  
      socket.on('trade', (msg) => {
        if (msg.type == 'bids') {
          delta_bids(msg.price, msg.quantity)
        } else if (msg.type == 'asks') {
          delta_asks(msg.price, msg.quantity)
        }
      })

      socket.on('match', (msg) => {
        delta_bids(msg.bid_price, -msg.quantity);
        delta_asks(msg.ask_price, -msg.quantity);
      })
    }
    socketInitializer();
  }, []);

  return (
    <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100vh' }}>
      <div style={{ width: '60%', height: '60%' }}>
        <OrderBook windowWidth={1080} asks={asks} bids={bids} />
      </div>
    </div>
  );
};

export default LandingPage;
