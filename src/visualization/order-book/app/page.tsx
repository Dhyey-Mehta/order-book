"use client"

import { useState, useEffect } from 'react';
import OrderBook from '@/app/OrderBook';
import MatchesBox from '@/app/MatchesBox';
import { io } from "socket.io-client";

const LandingPage = () => {
  const [bids, setBids] = useState([]);
  const [asks, setAsks] = useState([]);
  const [matches, setMatches] = useState([]);

  // handle incoming order or match
  const delta_bids = (price: number, volume: number) => {
        console.log("incoming bid:" + price + " " + volume)
        setBids((curr_bids) => {
          const index = curr_bids.findIndex((elem) => elem[0] == price);
          if (index > -1) {
            // Update the volume at the existing price point
            const updatedOrderBook = [...curr_bids]
            const new_value = updatedOrderBook[index][1] + volume;
            if (new_value == 0) {
              updatedOrderBook.splice(index, 1)
            } else {
              updatedOrderBook[index] = [price, new_value];
            }
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
        const new_value = updatedOrderBook[index][1] + volume;
        if (new_value == 0) {
          updatedOrderBook.splice(index, 1)
        } else {
          updatedOrderBook[index] = [price, new_value];
        }
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
        console.log("matched: %d of %s to %s @ %d", msg.quantity, msg.order1_id, msg.order2_id, msg.quantity)
        setMatches((curr_matches) => [
          ...curr_matches,
          msg
        ]);
      })
    }
    socketInitializer();
  }, []);

  return (
    <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100vh' }}>
      <div style={{ width: '60%', height: '60%' }}>
        <OrderBook windowWidth={1080} asks={asks} bids={bids} />
        <MatchesBox matches={matches} />
      </div>
    </div>
  );
};

export default LandingPage;
