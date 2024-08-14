
import { useRef } from 'react';
import {useStickyScroll} from 'use-chat-scroll'

const MatchesBox = ({ matches }) => {
  const containerRef = useRef<React.MutableRefObject<HTMLDivElement>>()
  useStickyScroll(containerRef, matches)
  return (
    <div ref={containerRef} style={{
      position: 'absolute',
      right: '20px',
      top: '20%',
      width: '18%', 
      height: '70%', 
      overflowY: 'auto', 
      backgroundColor: 'rgba(0, 0, 0, 0.8)', 
      padding: '10px', 
      borderRadius: '5px',
      boxShadow: '0 4px 8px rgba(0, 0, 0, 0.2)',
      color: 'white',
    }}>
      <h3 style={{ borderBottom: '1px solid #ccc', paddingBottom: '5px', marginBottom: '10px' }}>Latest Matches</h3>
      <ul style={{ listStyleType: 'none', padding: 0 }}>
        {matches.map((match, index) => (
          <li key={index} style={{ marginBottom: '10px' }}>
            Match: {match.order1_id} to {match.order2_id} @ {match.price} (qty: {match.quantity}) 
          </li>
        ))}
      </ul>
    </div>
  );
};

export default MatchesBox;