"use client"

import { useState } from 'react';
import OrderBook from './OrderBook';


const LandingPage = () => {
  const [bids, setBids] = useState([[18.00, 25]]);
  const [asks, setAsks] = useState([[20.00, 25]]);

  const delta = (side: string, price: number, volume: number) => {
    const updateOrderBook = (orderBook, setOrderBook) => {
      const index = orderBook.findIndex(([p]) => p === price);
      
      if (index > -1) {
        // Update the volume at the existing price point
        const updatedOrderBook = [...orderBook];
        updatedOrderBook[index][1] += volume;
        setOrderBook(updatedOrderBook);
      } else {
        // Insert new price point
        const updatedOrderBook = [...orderBook, [price, volume]];
        setOrderBook(updatedOrderBook);

      }
    };

    if (side === 'bids') {
      updateOrderBook(bids, setBids);
    } else if (side === 'asks') {
      updateOrderBook(asks, setAsks);
    }
  };

  return (
    <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100vh' }}>
      <div style={{ width: '60%', height: '60%' }}>
        <OrderBook windowWidth={1080} asks={asks} bids={bids}/>
      </div>
    </div>
  );
};

export default LandingPage;
