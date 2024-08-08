import React, { FunctionComponent, useEffect } from 'react';

import TitleRow from "./TitleRow";
import { Container, TableContainer } from "./styles";
import PriceLevelRow from "./PriceLevelRow";
import Spread from "../Spread";
import { MOBILE_WIDTH, ORDERBOOK_LEVELS } from "../constants";
import DepthVisualizer from "../DepthVisualizer";
import { PriceLevelRowContainer } from "./PriceLevelRow/styles";

export enum OrderType {
  BIDS,
  ASKS
}

interface OrderBookProps {
  windowWidth: number;
  bids: number[][];
  asks: number[][];
}

interface Delta {
  bids: number[][];
  asks: number[][];
}

const formatNumber = (arg: number): string => {
  return new Intl.NumberFormat('en-US').format(arg);
};

const transformArr = (arg: number[][]): number[][] => {
  const sumOfAllVolumes = arg.reduce((sum, [_, volume]) => sum + volume, 0);

  let cumulativeSum = 0;

  return arg.map(([price, volume]) => {
    cumulativeSum += volume;
    const total_scaled = (cumulativeSum / sumOfAllVolumes) * 100;
    return [price, volume, cumulativeSum, total_scaled];
  });
};


const OrderBook: FunctionComponent<OrderBookProps> = ({ windowWidth, bids, asks}) => {
  const formatPrice = (arg: number): string => {
    return arg.toLocaleString("en", { useGrouping: true, minimumFractionDigits: 2 })
  };

  const buildPriceLevels = (levels: number[][], orderType: OrderType = OrderType.BIDS): React.ReactNode => {
    let sortedLevelsByPrice: number[][] = [ ...levels ].sort(
      (currentLevel: number[], nextLevel: number[]): number => {
        let result: number = 0;
        if (orderType === OrderType.BIDS || windowWidth < MOBILE_WIDTH) {
          result = nextLevel[0] - currentLevel[0];
        } else {
          result = currentLevel[0] - nextLevel[0];
        }
        return result;
      }
    );

    const sumOfAllVolumes = sortedLevelsByPrice.reduce((sum, [_, volume]) => sum + volume, 0);
  
    let cumulativeSum = 0;
  
    sortedLevelsByPrice = sortedLevelsByPrice.map(([price, volume]) => {
      cumulativeSum += volume;
      const total_scaled = (cumulativeSum / sumOfAllVolumes) * 100;
      return [price, volume, cumulativeSum, total_scaled];
    });

    console.log(sortedLevelsByPrice)

    return (
      sortedLevelsByPrice.map((level, idx) => {
        const calculatedTotal: number = level[2];
        const total: string = formatNumber(calculatedTotal);
        const depth = level[3];
        const size: string = formatNumber(level[1]);
        const price: string = formatPrice(level[0]);

        return (
          <PriceLevelRowContainer key={idx + depth}>
            <DepthVisualizer key={depth} windowWidth={windowWidth} depth={depth} orderType={orderType} />
            <PriceLevelRow key={size + total}
                           total={total}
                           size={size}
                           price={price}
                           reversedFieldsOrder={orderType === OrderType.ASKS}
                           windowWidth={windowWidth} />
          </PriceLevelRowContainer>
        );
      })
    );
  };

  return (
    <Container>
        <>
          <TableContainer>
            {windowWidth > MOBILE_WIDTH && <TitleRow windowWidth={windowWidth} reversedFieldsOrder={false} />}
            <div>{buildPriceLevels(bids, OrderType.BIDS)}</div>
          </TableContainer>
          <Spread bids={bids} asks={asks} />
          <TableContainer>
            <TitleRow windowWidth={windowWidth} reversedFieldsOrder={true} />
            <div>
              {buildPriceLevels(asks, OrderType.ASKS)}
            </div>
          </TableContainer>
        </> 
    </Container>
  )
};

export default OrderBook;
