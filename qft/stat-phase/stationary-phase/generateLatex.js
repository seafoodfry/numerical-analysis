// generateLatex.js
const { renderToString } = require('katex');
const fs = require('fs');

// Define the LaTeX expression
const latexExpression = "Re[\\exp(i \\lambda x^2)]";

// Render it to HTML
const html = renderToString(latexExpression);

// Generate the TypeScript file that exports this HTML
const output = `export const yAxisLabelHTML = \`${html}\`;`;

// Save the output to a TypeScript file
fs.writeFileSync('src/generated/latexLabel.ts', output);
console.log("LaTeX formula rendered to HTML and saved to 'src/generated/latexLabel.ts'");
