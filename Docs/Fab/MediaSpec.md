# Fab Media Specification

## Current source image

- Repository path: `Docs/Images/4k-shot-toolbar.png`
- Dimensions: 2527 × 1497
- File size: 3,230,997 bytes
- SHA-256: `C49E38480210CC6188DF8F1B1CA2EFA554304B1861FEC9019B10D71C84D626EB`
- Fab status:
  - Resolution: Passes the minimum 1920 × 1080 requirement.
  - File size: Fails the under-3-MB requirement and must be optimized.

## Gallery image target

- Canvas: 1920 × 1080 or 2400 × 1350
- Format: PNG or high-quality JPEG
- Maximum file size: Under 3 MB
- Preserve the visible toolbar button, F12 tooltip, red circle, and arrow.
- Do not alter visible Unreal Editor UI text.
- Do not add Epic Games or Unreal Engine logos.

## Thumbnail target

- Canvas: 1920 × 1080
- Maximum file size: Under 3 MB
- Use the supplied toolbar screenshot as the primary visual.
- Keep the toolbar button and red annotation readable at thumbnail size.
- Recommended text:
  - `4K Screenshot Toolbar`
  - `One Click • F12 • 3840 × 2160`
- Keep text inside safe margins and avoid covering the toolbar button.
- Do not imply official Epic Games affiliation or endorsement.

## Final thumbnail

- Repository path: `Docs/Images/fab-thumbnail-1920x1080-v1.0.1.png`
- Dimensions: 1920 × 1080
- File size: 2,636,916 bytes
- SHA-256: `CA96668F6483D16A526BB5E939004B1E86B44E7BD6BEFA7922BD3C3851EB4C29`
- Fab status: Passes the minimum resolution and under-3-MB requirements.
- Production: Created with OpenAI `imagegen` from the supplied screenshot, then
  resized locally to the exact Fab target dimensions without adding new content.

## Image-generation/editing prompt

Create a polished Fab marketplace thumbnail from the supplied Unreal Editor screenshot. Preserve the screenshot content and the existing red circle and arrow exactly. Use a clean dark professional layout consistent with developer tools. Make the toolbar camera button the clear focal point. Add the exact title “4K Screenshot Toolbar” and subtitle “One Click • F12 • 3840 × 2160”. Keep all text crisp and fully inside safe margins. Do not change any visible Unreal Editor UI text, do not add fictional controls, and do not add Epic Games or Unreal Engine logos. Output at 1920 × 1080 and keep the final file under 3 MB.

## Remaining publisher action

Upload the final thumbnail in the Fab publisher portal and confirm the rendered
crop in both card and listing-preview views.
