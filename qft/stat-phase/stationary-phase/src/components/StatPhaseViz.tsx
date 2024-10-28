"use client";

import React, { useState } from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip } from 'recharts';
import { Card, CardHeader, CardTitle, CardContent } from '@/components/ui/card';
import { Slider } from '@/components/ui/slider';
import { BlockMath } from 'react-katex';
import 'katex/dist/katex.min.css';


const StatPhaseViz = () => {
  const [lambda, setLambda] = useState(1);

  // Generate data points
  const generateData = () => {
    const data = [];
    for (let x = -3; x <= 3; x += 0.05) {
      data.push({
        x: x,
        y: Math.cos(lambda * x * x)
      });
    }
    return data;
  };

  return (
    <Card className="w-full max-w-4xl">
      <CardHeader>
        <CardTitle>Method of Stationary Phase Visualization</CardTitle>
        <div className="text-sm text-gray-600">
          λ = {lambda.toFixed(1)}
        </div>
      </CardHeader>
      <CardContent>
        <BlockMath>
          {`I(\\lambda) = \\int_{-\\infty}^{\\infty} e^{i \\lambda x^2} \\, dx`}
        </BlockMath>

        <div className="mb-6">
          <p className="mb-2">Adjust λ to see how oscillations change:</p>
          <Slider 
            value={[lambda]}
            onValueChange={(vals) => setLambda(vals[0])}
            min={0.1}
            max={10}
            step={0.1}
            className="w-64"
          />
        </div>
        <LineChart width={800} height={300} data={generateData()} margin={{ top: 5, right: 20, bottom: 5, left: 20 }}>
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis 
            dataKey="x" 
            label={{ value: 'x', position: 'bottom' }} 
            tickFormatter={(value) => value.toFixed(2)} 
          />
          <YAxis
            domain={[-1.2, 1.2]}
            label={{
              value: 'Re[exp(iλx²)]',
              angle: -90,
              position: 'left',
              style: { textAnchor: 'middle' },
            }}
          />
          <Line type="monotone" dataKey="y" stroke="#8884d8" dot={false} />
          <Tooltip />
          {/* <Legend verticalAlign="top" align="right" /> */}
        </LineChart>
        <div className="mt-4 text-sm text-gray-600">
          Notice how the oscillations become more rapid away from x=0 (the stationary point) as λ increases.
          The integral&rsquo;s main contribution comes from the region near x=0 where the phase changes slowly.
        </div>
      </CardContent>
    </Card>
  );
};

export default StatPhaseViz;