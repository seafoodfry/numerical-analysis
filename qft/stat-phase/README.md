# React-based App

## Getting Started with a Framework
See latest framework recommendations in
[react.dev/start-a-new-react-project](https://react.dev/learn/start-a-new-react-project).

We will use next.js:

```
npx create-next-app@latest ./ --ts --tailwind --eslint --app --turbopack  --use-npm
```

Followed
[nextjs.org/static-exports](https://nextjs.org/docs/app/building-your-application/deploying/static-exports)

Added the following to `package.json`:

```json
"typecheck": "tsc --noEmit"
```

---

## Our App

We ran into trouble installing
[shadcn/ui](https://ui.shadcn.com/docs/installation/next)
so we downgraded react by changing `package.json` with the following:

```json
  "dependencies": {
    "react": "^18.3.1",
    "react-dom": "^18.3.1",
    ...
  },
```

Then running
```
npm install
```

Followed by
```
npm outdated
```
and
```
npm update
```

Then we ran

```
npx shadcn@latest init -d
```

We also install recharts:
```
npm install recharts
```

And we added the following components:

1. `npx shadcn@latest add card`
2. `npx shadcn@latest add slider`