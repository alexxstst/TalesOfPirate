do
    function file_exists(name)
		local f = io.open(name, "r")
		if (f ~= nil) then
			io.close(f)
			return true
		else
			return false
		end
	end

    function exportstring(s)
        s = string.format("%q", s)

        s = string.gsub(s, "\\\n", "\\n")
        s = string.gsub(s, "\r", "\\r")
        s = string.gsub(s, string.char(26), '"..string.char(26).."')
        return s
    end
	
    function table.getn(t)
        if t.n then
            return t.n
        else
            local n = 0
            for i in pairs(t) do
                if type(i) == "number" then
                    n = math.max(n, i)
                end
            end
            return n
        end
    end
	
    function table.save(tbl, filename)
        local charS, charE = "	", "\n"
        local file, err

        if not filename then
            file = {write = function(self, newstr)
                    self.str = self.str .. newstr
                end, str = ""}
            charS, charE = "", ""
        elseif filename == true or filename == 1 then
            charS, charE, file = "", "", io.tmpfile()
        else
            file, err = io.open(filename, "w")
            if err then
                return _, err
            end
        end

        local tables, lookup = {tbl}, {[tbl] = 1}

        file:write("return {" .. charE)
        for idx, t in ipairs(tables) do
            if filename and filename ~= true and filename ~= 1 then
                file:write("-- Table: {" .. idx .. "}" .. charE)
            end
            file:write("{" .. charE)
            local thandled = {}
            for i, v in ipairs(t) do
                thandled[i] = true

                if type(v) ~= "userdata" then
                    if type(v) == "table" then
                        if not lookup[v] then
                            table.insert(tables, v)
                            lookup[v] = #tables
                        end
                        file:write(charS .. "{" .. lookup[v] .. "}," .. charE)
                    elseif type(v) == "function" then
                        file:write(charS .. "loadstring(" .. exportstring(string.dump(v)) .. ")," .. charE)
                    else
                        local value = (type(v) == "string" and exportstring(v)) or tostring(v)
                        file:write(charS .. value .. "," .. charE)
                    end
                end
            end
            for i, v in pairs(t) do
                if (not thandled[i]) and type(v) ~= "userdata" then
                    if type(i) == "table" then
                        if not lookup[i] then
                            table.insert(tables, i)
                            lookup[i] = #tables
                        end
                        file:write(charS .. "[{" .. lookup[i] .. "}]=")
                    else
                        local index =
                            (type(i) == "string" and "[" .. exportstring(i) .. "]") or string.format("[%d]", i)
                        file:write(charS .. index .. "=")
                    end

                    if type(v) == "table" then
                        if not lookup[v] then
                            table.insert(tables, v)
                            lookup[v] = #tables
                        end
                        file:write("{" .. lookup[v] .. "}," .. charE)
                    elseif type(v) == "function" then
                        file:write("loadstring(" .. exportstring(string.dump(v)) .. ")," .. charE)
                    else
                        local value = (type(v) == "string" and exportstring(v)) or tostring(v)
                        file:write(value .. "," .. charE)
                    end
                end
            end
            file:write("}," .. charE)
        end
        file:write("}")

        if not filename then
            return file.str .. "--|"
        elseif filename == true or filename == 1 then
            file:seek("set")

            return file:read("*a") .. "--|"
        else
            file:close()
            return 1
        end
    end

    function table.load(sfile)
        if not file_exists(sfile) then
            local f = io.open(sfile, "a")
            table.save({}, sfile)
            f:close()
            return table.load(sfile)
        end

        if string.sub(sfile, -3, -1) == "--|" then
            tables, err = loadstring(sfile)
        else
            fd = nil
            while (fd == nil) do
                fd, err = io.open(sfile, "r")
            end
            tables, err = loadfile(sfile)
            io.close(fd)
        end
        if err then
            return _, err
        end
        tables = tables()
        for idx = 1, #tables do
            local tolinkv, tolinki = {}, {}
            for i, v in pairs(tables[idx]) do
                if type(v) == "table" and tables[v[1]] then
                    table.insert(tolinkv, {i, tables[v[1]]})
                end
                if type(i) == "table" and tables[i[1]] then
                    table.insert(tolinki, {i, tables[i[1]]})
                end
            end

            for _, v in ipairs(tolinkv) do
                tables[idx][v[1]] = v[2]
            end

            for _, v in ipairs(tolinki) do
                tables[idx][v[2]], tables[idx][v[1]] = tables[idx][v[1]], nil
            end
        end
        return tables[1]
    end
	
    function string.wrap(str, j, space)
        j = j or 42
        space = space or "_"
        local h = 1
        local s = ""
        local line = {}
        string.gsub(
            str,
            "(%s*)()(%S+)()",
            function(sp, st, word, fi)
                if (fi - h) > j then
                    h = st
                    table.insert(line, s)
                    s = word
                else
                    s = s .. sp .. word
                end
            end
        )
        if s ~= "" then
            table.insert(line, s)
        end
        local z = ""
        for i, v in pairs(line) do
            if i ~= #line then
                v = v .. space
            else
                v = v
            end
            z = z .. v
        end
        return z
    end
end
