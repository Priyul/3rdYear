<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
    <head>
    </head>
    <body style="background-color: black; color: white;">
      <h1>Abstract Syntax Tree</h1>
      <xsl:apply-templates select="//Node" />
    </body>
  </html>
</xsl:template>

<xsl:template match="Node">
  <xsl:variable name="indent" select="count(ancestor::Node) * 20"/>
  <div style="margin-left: {$indent}px;">
    <xsl:value-of select="@type" /> (<xsl:value-of select="@id" />)
    <xsl:if test="string-length(Contents) &gt; 0">
      : <xsl:value-of select="Contents" />
    </xsl:if>
    <br />
    <xsl:apply-templates select="ChildNodes/Node" />
  </div>
</xsl:template>

</xsl:stylesheet>
